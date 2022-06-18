/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 7 */
/* Aluno: Willian Pessoa        DRE: 116210965 */
/* Codigo: Soma de pares num vetor (concorrente) */
/* -------------------------------------------------------------------*/

import java.util.ArrayList;
import java.util.Random;

//classe da estrutura de dados (recurso) compartilhado entre as threads
class Data {
    private int m_currentIndex;
    private static ArrayList<Integer> m_data = null;

    public Data(ArrayList<Integer> data) {
        m_currentIndex = 0;
        // Variável estática compartilhada entre todos os objetos, não precisa ser atribuída mais de uma vez
        if (null == m_data) {
            m_data = data;
        }
    }

    public synchronized Integer getAndInc() {
        // Verificação necessária para não estourar o índice
        if (m_currentIndex >= m_data.size()) {
            return null;
        }
        return m_data.get(m_currentIndex++);
    }

    public synchronized Integer getElement(int index) {
        // Verificação necessária para não estourar o índice
        if (m_currentIndex >= m_data.size()) {
            return null;
        }
        return m_data.get(index);
    }

    public synchronized boolean isInTheEnd() {
        return m_currentIndex >= m_data.size();
    }

}

//classe que estende Thread e implementa a tarefa de cada thread do programa
class T extends Thread {
    //identificador da thread
    private int m_id;
    //objeto compartilhado com outras threads
    private Data m_data;
    private int m_evenFounded;

    //construtor
    public T(int tid, Data data) {
        m_id = tid;
        m_data = data;
        m_evenFounded = 0;
    }

    //metodo main da thread
    public void run() {
        while (!m_data.isInTheEnd()) {
            Integer n = m_data.getAndInc();
            //System.out.print("\nThread " + m_id + " checking " + n);
            if (n != null) { // Necessário pois pode ter havido condição de corrida não contemplada por este código
                if (n % 2 == 0) {
                    ++m_evenFounded;
                    //System.out.print(". Now 'evenFouded' is " + m_evenFounded);
                }
            }
        }
    }

    public int evenFounded() {
        return m_evenFounded;
    }
}

//classe da aplicacao
class Lab07 {
    static final int N_THREADS = 2;
    static final int RANGE = 1000;

    public static void main (String[] args) {
        //reserva espaço para um vetor de threads
        Thread[] threads = new Thread[N_THREADS];

        ArrayList<Integer> array = new ArrayList<Integer>();
        int evenFoundedSequentialy = 0;
        //System.out.print("[");
        Random rd = new Random();
        for (int i = 0; i < RANGE; ++i) {
             // creating Random object
            int n = rd.nextInt();
            //System.out.print(" " + n + ",");
            if (n % 2 == 0) {
                ++evenFoundedSequentialy;
            };
            array.add(n);
        }
        //System.out.println("]");

        //cria uma instancia do recurso compartilhado entre as threads
        Data data = new Data(array);

        //cria as threads da aplicacao
        for (int i=0; i<threads.length; i++) {
            threads[i] = new T(i, data);
        }

        //inicia as threads
        for (int i=0; i<threads.length; i++) {
            threads[i].start();
        }

        //espera pelo termino de todas as threads
        for (int i=0; i<threads.length; i++) {
            try { threads[i].join(); } catch (InterruptedException e) { return; }
        }

        int evenFoundedCuncurrently = 0;
        for (int i = 0; i < N_THREADS; ++i) {
            evenFoundedCuncurrently += ((T) threads[i]).evenFounded();
        }

        String ans = evenFoundedCuncurrently == evenFoundedSequentialy ? "YES!" : "NO!";
        System.out.println("\nBoth algorithms, sequential and concurrent," +
                " founded the same value?");
        System.out.println("---> " + ans);
    }
}

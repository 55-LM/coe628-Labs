/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package coe628.lab9;

/**
 *
 * @author a49alam
 */
public class lab9 {
    private static final int N = 5;
    private static final Semaphore[] forks = new Semaphore[N];
    private static final Semaphore room = new Semaphore(4);
    private static int completedDinners = 0;

    static {
        for (int i = 0; i < N; i++) {
            forks[i] = new Semaphore(1);
        }
    }

    static class Philosopher extends Thread {
        private final int id;
        private int mealsEaten = 0;
        private static final int MAX_MEALS = 1;

        public Philosopher(int id) {
            this.id = id;
        }

        public void run() {
            while (mealsEaten < MAX_MEALS) {
                try {
                    Thread.sleep(50); 
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }

                room.down(); // Enter the room

                int left = id;
                int right = (id + 4) % N; 

                if (!tryFork(left)) {
                    log("Philosopher " + (id + 1) + " is waiting for Fork " + (left + 1));
                    forks[left].down();
                }
                log("Fork " + (left + 1) + " taken by Philosopher " + (id + 1));

                if (!tryFork(right)) {
                    log("Philosopher " + (id + 1) + " is waiting for Fork " + (right + 1));
                    forks[right].down();
                }
                log("Fork " + (right + 1) + " taken by Philosopher " + (id + 1));

                log("Philosopher " + (id + 1) + " completed his dinner");

                // Release forks
                forks[right].up();
                forks[left].up();
                log("Philosopher " + (id + 1) + " released Fork " + (left + 1) + " and Fork " + (right + 1));

                mealsEaten++;
                room.up(); // Leave the room

                synchronized (lab9.class) {
                    completedDinners++;
                    log("Till now num of philosophers completed dinner are " + completedDinners);
                }

                try {
                    Thread.sleep(30); // Let others proceed
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }
        }

        private boolean tryFork(int forkId) {
            synchronized (forks[forkId]) {
                return forks[forkId].tryDown();
            }
        }

        private void log(String msg) {
            synchronized (System.out) {
                System.out.println(msg);
            }
        }
    }

    public static void main(String[] args) {
        for (int i = 0; i < N; i++) {
            new Philosopher(i).start();
        }
    }
}

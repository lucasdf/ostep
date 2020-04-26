class T1 {

  static class Counter {
    public int count = 0;
  }

  static class MyThread implements Runnable {
    String identifier;
    public MyThread(String identifier) {
      this.identifier = identifier;
    }
    public void run() {
      for (int i = 0; i < 1000000; i++) {
        counter.count = counter.count + 1;
      }
      System.out.format("%s: done\n", identifier);
    }
  }

  static Counter counter = new Counter();
  public static void main(String args[]) {
    System.out.format("main: begin (counter = %d)\n", counter.count);
    Thread thread1 = new Thread(new MyThread("A"));
    Thread thread2 = new Thread (new MyThread("B"));
    thread1.start();
    thread2.start();
    try{
      thread1.join();
      thread2.join();
    } catch (InterruptedException ex) {
      System.exit(1);
    }
    System.out.format("main: done with both (counter = %d)\n", counter.count);
  }
}

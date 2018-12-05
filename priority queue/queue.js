import java.util.Objects;
import java.util.PriorityQueue;

class Message implements Comparable<Message> {
    private String packet;
    private int priority;

    public Message(String packet, int priority) {
        this.packet = packet;
        this.priority = priority;
    }

    public String getPacket() {
        return packet;
    }

    public void setPacket(String packet) {
        this.packet = packet;
    }

    public double getPriority() {
        return priority;
    }

    public void setPriority(int priority) {
        this.priority = priority;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Message message = (Message) o;
        return Double.compare(message.priority, priority) == 0 &&
                Objects.equals(packet, message.packet);
    }

    @Override
    public int hashCode() {
        return Objects.hash(packet, priority);
    }

    @Override
    public String toString() {
        return "Message{" +
                "packet='" + packet + '\'' +
                ", priority=" + priority +
                '}';
    }

    // Compare two message packets by their priority value
    @Override
    public int compareTo(Message message) {
        if(this.getPriority() > message.getPriority()) {
            return 1;
        } else if (this.getPriority() < message.getPriority()) {
            return -1;
        } else {
            return 0;
        }
    }
}


public class PriorityQueueUserDefinedObjectExample {
    public static void main(String[] args) {
        /*
           The requirement for a PriorityQueue of user defined objects is that

           1. Either the class should implement the Comparable interface and provide
              the implementation for the compareTo() function.
           2. Or you should provide a custom Comparator while creating the PriorityQueue.
        */

        // Create a PriorityQueue
        PriorityQueue<Message> messagePriorityQueue = new PriorityQueue<>();

        // Add items to the Priority Queue
        messagePriorityQueue.add(new Message("Vision", 2));
        messagePriorityQueue.add(new Message("GPS", 3));
        messagePriorityQueue.add(new Message("Lidar", 1));
        messagePriorityQueue.add(new Message("App", 4));
        messagePriorityQueue.add(new Message("Vision", 2));
        messagePriorityQueue.add(new Message("GPS", 3));
        messagePriorityQueue.add(new Message("Lidar", 1));
        messagePriorityQueue.add(new Message("App", 4));

        /*
            The compareTo() method implemented in the Message class is used to determine
            in what order the objects should be dequeued.
        */
        while (!messagePriorityQueue.isEmpty()) {
            System.out.println(messagePriorityQueue.remove());
        }
    }
}

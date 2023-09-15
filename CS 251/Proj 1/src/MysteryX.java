class MysteryX {
    
    public class Node {
        int key;
        Node previous, next;
        
        public Node(int value) {
            key = value;
            previous = this;
            next = this;
        }
    }
    
    public Node head = null;
    
    public MysteryX() {
    }
    
    public void insertFront(int value) {
        Node insert = new Node(value);
        if ((head == null)) {
            head = insert;
        } else {
            insert.next = head;
            insert.previous = head.previous;
            head.previous.next = insert;
            head.previous = insert;
            head = insert;
        }
    }
    
    public void insertRear(int value) {
        Node insert = new Node(value);
        if ((head == null)) {
            head = insert;
        } else {
            insert.next = head;
            insert.previous = head.previous;
            head.previous.next = insert;
            head.previous = insert;
        }
    }
    
    public int deleteFront() {
        if (head == null) {
            return -1;
        }
        int value = head.key;
        if (head == head.previous) {
            head = null;
            return value;
        } else {
            head.previous.next = head.next;
            head.next.previous = head.previous;
            head = head.next;
        }
        return value;
    }
    
    public int deleteRear() {
        if (head == null) {
            return -1;
        }
        int value = head.previous.key;
        if (head == head.previous) {
            head = null;
            return value;
        } else {
            head.previous.previous.next = head;
            head.previous = head.previous.previous;
        }
        return value;
    }
    
    public int getFront() {
        if (head == null) {
            return -1;
        }
        return head.key;
    }
    
    public int getRear() {
        if (head == null) {
            return -1;
        }
        return head.previous.key;
    }
    
}


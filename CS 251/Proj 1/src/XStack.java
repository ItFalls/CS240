public class XStack {
    MysteryX stack = new MysteryX();
    
    public void push(int value) {
        stack.insertFront(value);
    }
    
    public int pop() {
        if(stack.head == null)
            return -1;
        else
            return stack.deleteFront();
    }
    
    public int peek() {
        if(stack.head == null)
            return -1;
        else
            return stack.head.key;
    }
}

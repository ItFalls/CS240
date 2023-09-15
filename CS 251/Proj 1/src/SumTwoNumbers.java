public class SumTwoNumbers {
    public class Node {
        int key;
        Node previous, next;
        public Node() {}
    }
    
    public Node num1_list;
    public Node num2_list;
    
    public Node add(String num1, String num2) {
        String[] one = num1.split("");
        String[] two = num2.split("");
        for (int x = 0; x < one.length; x++) {
            Node node = new Node();
            node.key = Integer.parseInt(one[x]);
            if (num1_list == null) {
                num1_list = node;
            } else {
                Node temp = num1_list;
                while (temp.next != null) {
                    temp = temp.next;
                }
                temp.next = node;
                node.previous = temp;
            }
        }
        for (int x = 0; x < two.length; x++) {
            Node node = new Node();
            node.key = Integer.parseInt(two[x]);
            if (num2_list == null) {
                num2_list = node;
            } else {
                Node temp = num2_list;
                while (temp.next != null) {
                    temp = temp.next;
                }
                temp.next = node;
                node.previous = temp;
            }
        }
        
        Node oneEnd = num1_list;
        while (oneEnd.next != null) {
            oneEnd = oneEnd.next;
        }
        
        Node twoEnd = num2_list;
        while (twoEnd.next != null) {
            twoEnd = twoEnd.next;
        }
        
        Node resultHead = null;
        int carry = 0;
        while (oneEnd != null || twoEnd != null || carry != 0) {
            int sum = carry;
            
            if (oneEnd != null) {
                sum += oneEnd.key;
                oneEnd = oneEnd.previous;
            }
            
            if (twoEnd != null) {
                sum += twoEnd.key;
                twoEnd = twoEnd.previous;
            }
            
            carry = sum / 10;
            int digit = sum % 10;
            
            Node newNode = new Node();
            newNode.key = digit;
            if (resultHead != null) {
                newNode.next = resultHead;
            }
            resultHead = newNode;
        }
        
        return resultHead;
    }
}
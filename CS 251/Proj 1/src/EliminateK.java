
public class EliminateK {
    
    public int computeSurvivor(int n, int k) {
        if (n == 0)
            return 0;
        if (k == 0)
            return n;
        
        int[] arr = new int[n];
        for (int x = 0; x < n; x++)
            arr[x] = x + 1;
        
        int index = 0;
        while (n > 1) {
            index = (index + k - 1) % n;
            for (int i = index; i < n - 1; i++) {
                arr[i] = arr[i + 1];
            }
            n--;
        }
        return arr[0];
    }
    
}

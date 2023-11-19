import java.io.*;
import java.util.*;

public class humancannonball {
    static class Pair implements Comparable<Pair> {
        int v;
        double t;

        public Pair(int vertex, double time) {
            v = vertex;
            t = time;
        }

        public int compareTo(Pair p) {
            return Double.compare(t, p.t);
        }
    }

    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));

        double[] source = Arrays.stream(br.readLine().split(" ")).mapToDouble(Double::parseDouble).toArray();
        double[] destination = Arrays.stream(br.readLine().split(" ")).mapToDouble(Double::parseDouble).toArray();
        int n = Integer.parseInt(br.readLine());
        double[][] coords = new double[n + 2][2];
        coords[0] = source;
        coords[n + 1] = destination;

        for (int i = 1; i < n + 1; i++) coords[i] = Arrays.stream(br.readLine().split(" ")).mapToDouble(Double::parseDouble).toArray();

        double[][] adjMatrix = new double[n + 2][n + 2];

        for (int i = 1; i < n + 2; i++)
            adjMatrix[0][i] = Math.hypot(coords[0][0] - coords[i][0], coords[0][1] - coords[i][1])/5;

        for (int i = 1; i < n + 1; i++)
            for (int j = 1; j < n + 2; j++)
                adjMatrix[i][j] = Math.min(2 + Math.abs(Math.hypot(coords[i][0] - coords[j][0], coords[i][1] - coords[j][1]) - 50)/5,
                        Math.hypot(coords[i][0] - coords[j][0], coords[i][1] - coords[j][1])/5);

        double[] time = new double[n + 2];
        Arrays.fill(time, Double.POSITIVE_INFINITY);
        time[0] = 0;

        PriorityQueue<Pair> pq = new PriorityQueue<>();
        pq.offer(new Pair(0, 0));
        while (!pq.isEmpty()) {
            Pair u = pq.poll();
            if (u.t != time[u.v]) continue;
            for (int v = 0; v < n + 2; v++) {
                double d = time[u.v] + adjMatrix[u.v][v];
                if (d < time[v]) {
                    time[v] = d;
                    pq.offer(new Pair(v, d));
                }
            }
        }
        System.out.printf("%3f", time[n + 1]);
    }
}
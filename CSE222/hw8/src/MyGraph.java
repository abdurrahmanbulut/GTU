import java.util.ArrayList;
import java.util.Iterator;
import java.util.LinkedList;

public class MyGraph implements DynamicGraph{
    ArrayList<Vertex> vertices;
    private int ids;

    public MyGraph(){
        vertices = new ArrayList<>();
        ids = 0;
    }

    public int getIndex(int s){
        int res = -1;
        for(int i = 0;i < this.vertices.size();i++){
            if(this.vertices.get(i).getId() == s){
                res = i;
                break;
            }
        }
        return res;
    }

    @Override
    public int getNumV() {
        return this.vertices.size();
    }

    @Override
    public boolean isDirected() {
        return false;
    }

    @Override
    public void removeVertex(String s){
        for(int i = 0; i < this.vertices.size();i++){
            if(this.vertices.get(i).getLabel().equals(s)){
                removeVertex(this.vertices.get(i).getId());
            }
        }

    }

    @Override
    public void insert(Edge edge) {
        this.vertices.get(edge.getV1().getId()).getEdges().add(edge);
        Edge e2 = new Edge(edge.getV2(),edge.getV1(),edge.getWeight());
        this.vertices.get(edge.getV2().getId()).getEdges().add(e2);

    }

    @Override
    public boolean isEdge(int s, int d) {
        boolean res = false;
        for(int i = 0; i < this.vertices.get(getIndex(s)).getEdges().size();i++){
            if(this.vertices.get(getIndex(s)).getEdges().get(i).getV2().getId() == d){
                res = true;
                break;
            }
        }
        return res;
    }

    @Override
    public Edge getEdge(int s, int d) {
        Edge res = null;
        if(isEdge(s,d)){
            for(int i = 0; i < this.vertices.get(getIndex(s)).getEdges().size();i++){
                if(this.vertices.get(getIndex(s)).getEdges().get(i).getV2().getId() == d){
                    res = this.vertices.get(getIndex(s)).getEdges().get(i);
                    break;
                }
            }
        }
        return res;
    }

    @Override
    public Iterator<Edge> edgeIterator(int s) {
        Iterator<Edge> it = new Iterator<Edge>() {
            private int currin = 0;
            @Override
            public boolean hasNext() {
                return currin < vertices.get(getIndex(s)).getEdges().size()-1;
            }

            @Override
            public Edge next() {
                return vertices.get(getIndex(s)).getEdges().get(currin++);
            }
        };
        return it;
    }

    @Override
    public Vertex newVertex(String l, int w) {
        return new Vertex(l,ids++,w);
    }

    @Override
    public void addVertex(Vertex v) {
        this.vertices.add(v);

    }

    @Override
    public void addEdge(int s, int d, double w) {
        Edge e1 = new Edge(this.vertices.get(getIndex(s)),this.vertices.get(getIndex(d)),w);
        Edge e2 = new Edge(this.vertices.get(getIndex(d)),this.vertices.get(getIndex(s)),w);
        this.vertices.get(getIndex(s)).getEdges().add(e1);
        this.vertices.get(getIndex(d)).getEdges().add(e2);
    }

    @Override
    public void removeEdge(int s, int d) {
        if(isEdge(s,d)){
            Edge e1 = getEdge(s,d);
            Edge e2 = getEdge(d,s);
            this.vertices.get(getIndex(s)).getEdges().remove(e1);
            this.vertices.get(getIndex(d)).getEdges().remove(e2);
        }
    }

    @Override
    public void removeVertex(int s) {
        while (this.vertices.get(getIndex(s)).getEdges().size() != 0){
            Edge e1 = this.vertices.get(getIndex(s)).getEdges().get(0);
            removeEdge(s,e1.getV2().getId());
        }
        this.vertices.remove(getIndex(s));
    }

    @Override
    public MyGraph filterVertices(String key, String filter) {

        if(key.equals("label")){
            MyGraph my = new MyGraph();
            for(int i = 0; i< this.vertices.size();i++){
                Vertex v = this.vertices.get(i);
                Vertex nv = new Vertex(v.getLabel(),v.getId(),v.getWeight());
                if(nv.getLabel().equals(filter)){
                    my.addVertex(nv);
                }

            }

            for(int i = 0; i < my.vertices.size();i++){
                Vertex nv = my.vertices.get(i);
                Vertex v2 = null;
                for(int j = 0; j < this.vertices.get(getIndex(nv.getId())).getEdges().size();j++){
                    if(this.vertices.get(getIndex(nv.getId())).getEdges().get(j).getV2().getLabel().equals(filter)){
                        v2 = my.vertices.get(my.getIndex(this.vertices.get(getIndex(nv.getId())).getEdges().get(j).getV2().getId()));
                        Edge e1 = new Edge(nv,v2,this.vertices.get(getIndex(nv.getId())).getEdges().get(j).getWeight());

                        nv.getEdges().add(e1);

                    }
                }

            }

            return my;
        }
        else {
            return null;
        }
    }

    @Override
    public int[][] exportMatrix() {
        int[][] d = new int[getNumV()][getNumV()];
        for(int i = 0; i < getNumV();i++){
            for(int j = 0; j < getNumV();j++){
                if(i == j){
                    d[i][j] = 0;
                }else{
                    if(getEdge(this.vertices.get(i).getId(),this.vertices.get(j).getId()) != null){
                        d[i][j] = (int)getEdge(this.vertices.get(i).getId(),this.vertices.get(j).getId()).getWeight();
                    }else{
                        d[i][j]  = 0;
                    }
                }
            }

        }
        return d;
    }
    public double BFS(MyGraph my){
        boolean[] visited = new boolean[my.vertices.size()];
        double cost = 0;
        visited[0] = true;
        LinkedList<Vertex> que = new LinkedList<>();
        que.add(my.vertices.get(0));

        while (que.size() != 0){
            Vertex v = que.poll();

            for(int i = 0; i < v.getEdges().size();i++){
                int ind = my.getIndex(v.getEdges().get(i).getV2().getId());
                if(!visited[ind]){
                    visited[ind] = true;
                    cost += v.getEdges().get(i).getWeight();
                    que.add(my.vertices.get(ind));
                }
            }
        }
       return cost;
    }

    private double dfs(MyGraph my, Vertex v, boolean[] visited){
        int ind = my.getIndex(v.getId());
        visited[ind] = true;
        double cost = 0;

        for(int i = 0; i < v.getEdges().size();i++){
            int n = my.getIndex(v.getEdges().get(i).getV2().getId());
            if(!visited[n]){
                cost += v.getEdges().get(i).getWeight();
                cost += dfs(my,my.vertices.get(n),visited);
            }
        }
        return cost;
    }
    public double DFS(MyGraph my){
        double c = 0;
        boolean[] visited = new boolean[my.getNumV()];
        c = dfs(my,my.vertices.get(0),visited);

        return c;


    }

    private int minDist(MyGraph myGraph, int[] dist, boolean[] visited){
        int min = Integer.MAX_VALUE,ind = -1;

        for(int i = 0; i < myGraph.getNumV();i++){
            if(!visited[i] && dist[i] < min){
                min = dist[i];
                ind = i;

            }
        }
        return ind;
    }

    public void dijkstra(MyGraph myGraph,Vertex v){
        int[] dist = new int[myGraph.getNumV()];
        boolean[] vis = new boolean[myGraph.getNumV()];
        for(int i = 0; i< myGraph.getNumV();i++){
            dist[i] = Integer.MAX_VALUE;
            vis[i] = false;
        }
        dist[myGraph.getIndex(v.getId())] = 0;
        int head = myGraph.getIndex(v.getId());

        for(int c = 0; c < myGraph.getNumV()-1;c++){
            int u = minDist(myGraph,dist,vis);
            vis[u] = true;
            for(int cc = 0 ; cc < myGraph.getNumV();cc++){
                int nv = dist[u] + myGraph.exportMatrix()[u][cc];

                if(u != head && u != cc && !vis[cc]){
                    nv -= myGraph.vertices.get(u).getBoost();

                }

                if(!vis[cc] && myGraph.exportMatrix()[u][cc] != 0 && dist[u] != Integer.MAX_VALUE && nv < dist[cc]){
                    dist[cc] = nv;

                }
            }
        }

        for (int i = 0; i< dist.length;i++){
            if(dist[i] != 0){
                System.out.println("From vertex " + v.getId() +" to vertex " + myGraph.vertices.get(i).getId() +" distance is : " +dist[i] );

            }
        }
    }


    @Override
    public void printGraph() {
        for(int i = 0; i< this.vertices.size();i++){
            System.out.println(this.vertices.get(i).getId() + " => " );
            for (int j =0 ; j < this.vertices.get(i).getEdges().size();j++){
                System.out.print(" -> " + this.vertices.get(i).getEdges().get(j).getV2().getId() );
            }
            System.out.println();
        }


    }
}

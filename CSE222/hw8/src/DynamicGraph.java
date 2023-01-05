

public interface DynamicGraph extends Graph{

    Vertex newVertex(String l,int w);

    void addVertex(Vertex v);

    void addEdge(int s,int d,double w);

    void removeEdge(int s,int d);

    void removeVertex(int s);

    void removeVertex(String label);

    MyGraph filterVertices(String l,String key);

    int[][] exportMatrix();

    void printGraph();

}

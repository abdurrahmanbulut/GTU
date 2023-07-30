public class Main {

    public static void main(String[] args){

        MyGraph myGraph = new MyGraph();
        System.out.println("Graph is created..");
        Vertex v1 = myGraph.newVertex("ada",4);
        System.out.println("new vertex1 added , string : 'ada' with weight 4");
        Vertex v2 = myGraph.newVertex("a",8);
        System.out.println("new vertex2 added , string :  'a' with weight 8");
        Vertex v3 = myGraph.newVertex("a",8);
        System.out.println("new vertex3 added string : 'a' with weight 8");

        v2.setBoost(3);
        System.out.println("Set boost to 3 in vertex2");
        v1.setBoost(2);
        System.out.println("Set boost to 2 in vertex1");
        v3.setBoost(3);
        System.out.println("Set boost to 3 in vertex3");

        myGraph.addVertex(v1);
        System.out.println("Vertex1 added to graph");
        myGraph.addVertex(v2);
        System.out.println("Vertex2 added to graph");
        myGraph.addVertex(v3);
        System.out.println("Vertex3 added to graph");

        myGraph.addEdge(v1.getId(), v2.getId(), 4);
        System.out.println("Edge added between vertex1 and vertex2 with weight 4");
        myGraph.addEdge(v1.getId(), v3.getId(), 8);
        System.out.println("Edge added between vertex1 and vertex3 with weight 8");
        myGraph.addEdge(v2.getId(), v3.getId(), 6);
        System.out.println("Edge added between vertex2 and vertex3 with weight 6");

        myGraph.printGraph();
        System.out.println("Graph is printed");
        System.out.println("\n\n");

        MyGraph m2 = myGraph.filterVertices("label","a");

        m2.printGraph();
        System.out.println("\n");
        myGraph.printGraph();

        System.out.println(traversalDiff(myGraph));

        myGraph.exportMatrix();
        dijkstra(myGraph,v1);

        myGraph.removeVertex("ada");
        myGraph.printGraph();
        myGraph.addVertex(v1);
        myGraph.addEdge(v1.getId(), v2.getId(), 4);
        myGraph.addEdge(v1.getId(),v3.getId(),6);
        myGraph.printGraph();

        dijkstra(myGraph,v1);


    }

    public static double traversalDiff(MyGraph my){
        double c1 = my.BFS(my);
        double c2 = my.DFS(my);
        return c1-c2;

    }

    public static void dijkstra(MyGraph myGraph,Vertex v){
        myGraph.dijkstra(myGraph,v);
    }
}

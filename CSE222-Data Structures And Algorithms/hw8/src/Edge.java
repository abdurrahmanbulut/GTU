public class Edge {

    double weight;
    Vertex v1;
    Vertex v2;

    public Edge(Vertex v1,Vertex v2,double w){
        this.v1 = v1;
        this.v2 = v2;
        this.weight = w;

    }

    public double getWeight() {
        return weight;
    }

    public void setWeight(double weight) {
        this.weight = weight;
    }

    public Vertex getV1() {
        return v1;
    }

    public void setV1(Vertex v1) {
        this.v1 = v1;
    }

    public Vertex getV2() {
        return v2;
    }

    public void setV2(Vertex v2) {
        this.v2 = v2;
    }
}

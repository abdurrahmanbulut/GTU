import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;

public class Vertex {

    String label;
    int id;
    int weight;
    int boost;

    ArrayList<Edge> edges;

    public Vertex(String l,int i,int w){
        this.label = l;
        this.id = i;
        this.weight = w;
        this.boost = 0;
        this.edges = new ArrayList<>();
    }

    public String getLabel() {
        return label;
    }

    public void setLabel(String label) {
        this.label = label;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getWeight() {
        return weight;
    }

    public void setWeight(int weight) {
        this.weight = weight;
    }

    public int getBoost() {
        return boost;
    }

    public void setBoost(int boost) {
        this.boost = boost;
    }

    public ArrayList<Edge> getEdges() {
        Collections.sort(this.edges, Comparator.comparing(Edge::getWeight));
        return edges;
    }
}

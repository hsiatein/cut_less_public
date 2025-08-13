package JavaBinding;

public class Node {
    public int[] size;      // 长度 3
    public long partTypeID; // 对应 C++ size_t
    public Node[] childs;   // 子节点
    public Orient nextCutOrient;

    public Node(int[] size, long partTypeID, Node[] childs, Orient nextCutOrient) {
        this.size = size;
        this.partTypeID = partTypeID;
        this.childs = childs;
        this.nextCutOrient = nextCutOrient;
    }

    public enum Orient {
        X,
        Y,
        Z,
        NONE,
    }
}
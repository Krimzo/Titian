package named;

import java.io.Serializable;

public abstract class Named implements Serializable {
    private final NameHolder holder;
    private String name;

    public Named(NameHolder holder) {
        this(holder, "Unknown");
    }

    public Named(NameHolder holder, String name) {
        this.holder = holder;
        setName(name);
    }

    public void eraseName() {
        holder.names.remove(name);
        name = null;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        String possibleName = name;
        int nameCounter = 0;

        holder.names.remove(this.name);
        while (holder.names.contains(possibleName)) {
            possibleName = name + ++nameCounter;
        }

        holder.names.add(possibleName);
        this.name = possibleName;
    }
}

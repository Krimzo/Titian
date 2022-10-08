package named;

import java.io.Serializable;

public abstract class Named implements Serializable {
    private String name = null;
    private final NameHolder holder;

    public Named(NameHolder holder, String name) {
        if (holder != null && name == null) {
            throw new Error("Name must be provided to a Named object if NameHolder was provided!");
        }

        this.holder = holder;
        setName(name);
    }

    public void eraseName() {
        if (holder != null) {
            holder.names.remove(name);
        }
        name = null;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        String possibleName = name;

        if (holder != null) {
            holder.names.remove(this.name);

            for (int i = 1; holder.names.contains(possibleName); i++) {
                possibleName = name + i;
            }

            holder.names.add(possibleName);
        }

        this.name = possibleName;
    }
}

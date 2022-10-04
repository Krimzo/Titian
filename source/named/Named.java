package named;

import java.io.Serializable;

public abstract class Named implements Serializable {
    private String name = "Orphan Named";
    private final NameHolder holder;

    public Named(NameHolder holder, String name) {
        this.holder = holder;
        if (holder != null) {
            if (name != null) {
                setName(name);
            }
            else {
                throw new Error("Name must be provided to a Named object if NameHolder was provided!");
            }
        }
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

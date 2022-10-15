package named;

import utility.Instance;

import java.io.Serializable;

public abstract class Named implements Serializable {
    private final NameHolder holder;
    private String name = null;

    public Named(NameHolder holder, String name) {
        this.holder = holder;
        setName(name);
    }

    public void eraseName() {
        if (Instance.isValid(holder)) {
            holder.names.remove(name);
        }
        name = null;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        String possibleName = name;

        if (Instance.isValid(holder)) {
            if (Instance.isNull(name)) {
                throw new Error("Name can't be null if Named object references a NameHolder");
            }

            holder.names.remove(this.name);
            for (int i = 1; holder.names.contains(possibleName); i++) {
                possibleName = name + i;
            }
            holder.names.add(possibleName);
        }

        this.name = possibleName;
    }
}

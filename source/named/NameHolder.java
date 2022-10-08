package named;

import utility.nncollection.NNHashSet;

import java.io.Serializable;
import java.util.Set;

public final class NameHolder implements Serializable {
    final Set<String> names = new NNHashSet<>();

    public NameHolder() {}

    public void clear() {
        names.clear();
    }
}

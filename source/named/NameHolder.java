package named;

import java.io.Serializable;
import java.util.HashSet;
import java.util.Set;

public final class NameHolder implements Serializable {
    final Set<String> names = new HashSet<>();

    public NameHolder() {}

    public void clear() {
        names.clear();
    }
}

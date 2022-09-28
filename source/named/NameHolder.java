package named;

import interfaces.Disposable;

import java.io.Serializable;
import java.util.HashSet;
import java.util.Set;

public final class NameHolder implements Disposable, Serializable {
    final Set<String> names = new HashSet<>();

    public NameHolder() {}

    @Override
    public void dispose() {
        names.clear();
    }
}

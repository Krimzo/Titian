package entity.abs;

import entity.Entity;
import gui.abs.GUIRenderable;

import java.io.Serializable;

public abstract class EntityComponent implements GUIRenderable, Serializable {
    protected final Entity entity;

    protected EntityComponent(Entity entity) {
        this.entity = entity;
    }
}

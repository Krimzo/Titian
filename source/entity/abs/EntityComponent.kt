package entity.abs

import entity.Entity
import gui.abs.GUIRenderable
import java.io.Serializable

abstract class EntityComponent protected constructor(protected val entity: Entity) : GUIRenderable, Serializable

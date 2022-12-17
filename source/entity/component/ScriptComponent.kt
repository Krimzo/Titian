package entity.component

import entity.Entity
import entity.abs.EntityComponent
import imgui.ImGui
import script.Script
import java.io.Serializable
import java.util.concurrent.atomic.AtomicInteger

class ScriptComponent(entity: Entity) : EntityComponent(entity), Serializable {
    var scripts: ArrayList<Script> = ArrayList()

    fun reload() {
        for (script in scripts) {
            script.reload(entity)
        }
    }

    override fun renderGUI() {
        val i = AtomicInteger()
        while (i.get() < scripts.size) {
            ImGui.pushID(i.get())
            scripts[i.get()].renderGUI(scripts, i)
            ImGui.popID()
            i.getAndIncrement()
        }
    }
}
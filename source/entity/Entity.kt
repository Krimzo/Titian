package entity

import editor.Editor
import glparts.Shaders
import imgui.ImGui
import named.NameHolder
import named.Named
import physics.abs.Physical
import renderer.abs.Renderable
import java.io.Serializable

open class Entity(holder: NameHolder?, name: String?, @field:Transient var editor: Editor?) : Named(holder, name), Physical, Renderable, Serializable {
    val components = EntityComponents(this)

    fun callScriptStarts() {
        for (script in components.script.scripts) {
            script!!.callStarts()
        }
    }

    fun callScriptUpdates() {
        for (script in components.script.scripts) {
            script!!.callUpdates()
        }
    }

    override fun updatePhysics(deltaT: Float) {
        components.physics.velocity.set(components.physics.velocity.add(components.physics.acceleration.multiply(deltaT)))
        components.transform.position.set(components.transform.position.add(components.physics.velocity.multiply(deltaT)))
        components.transform.rotation.set(components.transform.rotation.add(components.physics.angular.multiply(deltaT)))
    }

    override fun gameRender(shaders: Shaders) {
        shaders.setUniform("W", components.transform.matrix())
        components.material.use {
            components.material.material!!.updateUniforms(shaders)
            components.mesh.gameRender(shaders)
        }
    }

    override fun editorRender(shaders: Shaders) {
        shaders.setUniform("W", components.transform.matrix())
        components.material.use {
            components.material.material!!.updateUniforms(shaders)
            components.mesh.editorRender(shaders)
        }
    }

    override fun indexRender(shaders: Shaders) {
        shaders.setUniform("W", components.transform.matrix())
        components.material.use {
            components.mesh.indexRender(shaders)
        }
    }

    open fun renderInfoGUI(editor: Editor) {
        ImGui.bulletText(name)
    }
}
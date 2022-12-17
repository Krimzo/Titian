package script

import entity.Entity
import gui.helper.GUIEdit
import gui.helper.GUIPopup
import imgui.ImGui
import script.abs.Scriptable
import utility.helper.FileHelper
import java.io.ObjectInputStream
import java.io.Serial
import java.io.Serializable
import java.lang.reflect.Field
import java.util.concurrent.atomic.AtomicInteger

class Script : Serializable {
    private var filepath: String? = null
    private var name: String? = null
    private var data: ByteArray? = null
    private var entity: Entity? = null

    @Transient
    private var type: Class<*>? = null

    @Transient
    private var instance: Scriptable? = null

    @Transient
    private var fields: ArrayList<Field>? = null

    constructor(name: String, data: ByteArray, entity: Entity) {
        load(name, data, entity)
    }

    constructor(filepath: String, entity: Entity) {
        load(filepath, entity)
    }

    @Serial
    private fun readObject(stream: ObjectInputStream) {
        stream.defaultReadObject()
        name?.let { name ->
            data?.let { data ->
                loadType(name, data)
            }
        }
        entity?.let {
            loadInstance(it)
        }
        loadFields()
    }

    fun load(name: String, data: ByteArray, entity: Entity) {
        try {
            this.name = name
            this.data = data.copyOf()
            this.entity = entity

            loadType(name, data)
            loadInstance(entity)
            loadFields()
        }
        catch (ignored: Exception) {
            println("Script \"$name\" loading error")
        }
    }

    fun load(filepath: String, entity: Entity) {
        this.filepath = filepath
        ScriptCompiler().compile(filepath)?.let { data ->
            FileHelper.getNameWithoutExtension(filepath)?.let { name ->
                load(name, data, entity)
            }
        }
    }

    fun reload(entity: Entity) {
        filepath?.let {
            load(it, entity)
        }
    }

    private fun loadType(name: String, data: ByteArray) {
        type = try {
            ScriptLoader().load(name, data)
        }
        catch (ignored: Exception) {
            println("Script \"$name\" type loading error")
            null
        }
    }

    private fun loadInstance(entity: Entity) {
        instance = try {
            type?.getDeclaredConstructor(Entity::class.java)?.let {
                it.newInstance(entity) as Scriptable
            }
        }
        catch (ignored: Exception) {
            println("Script \"$name\" instance creation error")
            null
        }
    }

    private fun loadFields() {
        try {
            fields = ArrayList()
            type?.let {
                for (field in it.declaredFields) {
                    if (field.canAccess(instance)) {
                        fields?.add(field)
                    }
                }
            }
        }
        catch (ignored: Exception) {
            println("Script \"$name\" fields loading error")
            fields = null
        }
    }

    fun callStarts() {
        try {
            instance?.start()
        }
        catch (ignored: Exception) {
            println("Script \"$name\" start call error")
        }
    }

    fun callUpdates() {
        try {
            instance?.update()
        }
        catch (ignored: Exception) {
            println("Script \"$name\" update call error")
        }
    }

    fun renderGUI(scripts: ArrayList<Script>, i: AtomicInteger) {
        val valid = fields != null && instance != null
        val headerState = ImGui.collapsingHeader(if (valid) name else "$name?")

        GUIPopup.itemPopup("EditScripts$i") {
            if (ImGui.button("Delete")) {
                scripts.removeAt(i.getAndDecrement())
                GUIPopup.close()
            }
        }

        if (headerState && valid) {
            fields?.let { fields ->
                for (field in fields) {
                    try {
                        instance?.let {
                            GUIEdit.editField(field, it)
                        }
                    }
                    catch (ignored: Exception) {
                        println("Script \"$name\" field gui error")
                    }
                }
            }
        }
    }

    companion object {
        fun getTemplate(className: String): String {
            return """import script.abs.Scriptable;
import entity.Entity;
import window.input.*;
import java.awt.Color;
import math.*;

public class $className extends Scriptable {
    public $className(Entity entity) {
        super(entity);
    }

    @Override
    public void start() {

    }

    @Override
    public void update() {

    }
}
"""
        }
    }
}
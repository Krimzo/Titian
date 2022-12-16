package camera.abs

import editor.Editor
import entity.Entity
import gui.helper.GUIEdit
import imgui.ImGui
import math.Float2
import math.Float3
import math.Int2
import math.Mat4
import named.NameHolder
import window.input.Input
import java.io.Serializable

abstract class Camera(holder: NameHolder?, name: String?, editor: Editor?) : Entity(holder, name, editor), Serializable {
    private var forward: Float3? = Float3.negZ
    private var right: Float3? = Float3.posX
    private var up: Float3? = Float3.posY

    var near = 0.01f
    var far = 500f
    var speed = 2f
    var sensitivity = 0.1f
    var background = Float3(0.1f)

    private var firstClick = true
    private var camMoving = false

    fun setForward(forward: Float3?) {
        this.forward = forward!!.normalize()
        right = this.forward!!.cross(up)
    }

    fun setUp(up: Float3) {
        this.up = up.normalize()
        right = forward!!.cross(this.up)
    }

    fun getForward(): Float3 {
        return Float3(forward)
    }

    fun getRight(): Float3 {
        return Float3(right)
    }

    fun getUp(): Float3 {
        return Float3(up)
    }

    fun viewMatrix(): Mat4 {
        return Mat4.Companion.lookAt(components.transform.position, components.transform.position.add(forward), up)
    }

    abstract fun projectionMatrix(): Mat4
    abstract fun matrix(): Mat4?
    fun moveForward(deltaTime: Float) {
        components.transform.position.set(components.transform.position.add(forward!!.multiply(speed * deltaTime)))
    }

    fun moveBack(deltaTime: Float) {
        components.transform.position.set(components.transform.position.subtract(forward!!.multiply(speed * deltaTime)))
    }

    fun moveRight(deltaTime: Float) {
        components.transform.position.set(components.transform.position.add(right!!.multiply(speed * deltaTime)))
    }

    fun moveLeft(deltaTime: Float) {
        components.transform.position.set(components.transform.position.subtract(right!!.multiply(speed * deltaTime)))
    }

    fun moveUp(deltaTime: Float) {
        components.transform.position.set(components.transform.position.add(up!!.multiply(speed * deltaTime)))
    }

    fun moveDown(deltaTime: Float) {
        components.transform.position.set(components.transform.position.subtract(up!!.multiply(speed * deltaTime)))
    }

    fun rotate(mousePos: Int2?, frameCenter: Int2?, angleLimit: Float) {
        val delta = mousePos!!.subtract(frameCenter)
        val rotation = Float2(delta).multiply(sensitivity)
        val forwardVert = forward!!.rotate(-rotation!!.y, right)
        if (Math.abs(forwardVert!!.angle(up) - 90) <= angleLimit) {
            forward = forwardVert
        }
        setForward(forward!!.rotate(-rotation.x, up))
    }

    fun useDefaultMovement(mouse: Int, forward: Int, back: Int, right: Int, left: Int, up: Int, down: Int, multi: Float, frameSize: Int2, deltaT: Float) {
        val speed = speed
        if (Input.isShiftDown) {
            this.speed *= multi
        }
        if (Input.isKeyDown(forward)) {
            moveForward(deltaT)
        }
        if (Input.isKeyDown(back)) {
            moveBack(deltaT)
        }
        if (Input.isKeyDown(right)) {
            moveRight(deltaT)
        }
        if (Input.isKeyDown(left)) {
            moveLeft(deltaT)
        }
        if (Input.isKeyDown(up)) {
            moveUp(deltaT)
        }
        if (Input.isKeyDown(down)) {
            moveDown(deltaT)
        }
        this.speed = speed
        if (Input.isMousePressed(mouse)) {
            editor!!.window.setMouseState(false)
            camMoving = true
        }
        if (Input.isMouseDown(mouse)) {
            if (camMoving) {
                val frameCenter = frameSize.divide(2)
                if (!firstClick) {
                    rotate(editor!!.window.mousePosition, frameCenter, 85f)
                }
                firstClick = false
                editor!!.window.mousePosition = frameCenter
            }
        }
        if (Input.isMouseReleased(mouse)) {
            editor!!.window.setMouseState(true)
            firstClick = true
            camMoving = false
        }
    }

    override fun renderInfoGUI(editor: Editor) {
        super.renderInfoGUI(editor)
        val isMain = editor.scene.selected.camera === this
        if (ImGui.checkbox("Main camera", isMain)) {
            editor.scene.selected.camera = if (isMain) null else this
        }
        GUIEdit.editFloat3("Forward", forward, 0.01f)
        setForward(forward)
        near = GUIEdit.editFloat("Near plane", near, 0.05f)
        far = GUIEdit.editFloat("Far plane", far, 0.05f)
        speed = GUIEdit.editFloat("Speed", speed, 0.05f)
        sensitivity = GUIEdit.editFloat("Sensitivity", sensitivity, 0.05f)
        GUIEdit.editColor3("Background", background)
    }
}
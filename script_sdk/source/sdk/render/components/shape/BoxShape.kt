package sdk.render.components.shape

import sdk.math.vector.Float3

class BoxShape : Shape(ShapeType.BOX) {
    var size: Float3 = Float3(1f)

    override fun getGeometry(): Float3 {
        return size
    }
}

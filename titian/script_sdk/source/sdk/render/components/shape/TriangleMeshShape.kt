package sdk.render.components.shape

import sdk.math.vector.Float3

class TriangleMeshShape : Shape(ShapeType.TRIANGLE_MESH) {
    override fun getGeometry(): Float3 {
        return Float3(-1f)
    }
}

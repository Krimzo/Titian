import script.abs.Scriptable;
import entity.Entity;
import java.awt.Color;
import math.*;

public class FieldsTest extends Scriptable {
    public boolean someBool = false;
    public byte someByte = 1;
    public short someShort = 2;
    public int someInt = 3;
    public long someLong = 4;
    public float someFloat = 5.0f;
    public double someDouble = 6.0;
    public char someChar = 'C';

    public Int2 someInt2 = new Int2(1, 2);
    public Int3 someInt3 = new Int3(1, 2, 3);
    public Int4 someInt4 = new Int4(1, 2, 3, 4);
    public Float2 someFloat2 = new Float2(1, 2);
    public Float3 someFloat3 = new Float3(1, 2, 3);
    public Float4 someFloat4 = new Float4(1, 2, 3, 4);
    public Color someColor = new Color(218, 122, 50);
    public String someString = "test";

    public String someNullField = null;

    public FieldsTest(Entity entity) {
        super(entity);
    }

    @Override
    public void start() {

    }

    @Override
    public void update() {

    }
}

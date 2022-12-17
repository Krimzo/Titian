package utility.helper

object StringHelper {
    private val spaces: HashMap<Int, String> = HashMap()

    fun spaces(count: Int): String {
        if (count < 1) {
            return ""
        }

        return if (spaces.containsKey(count)) {
            spaces[count] ?: ""
        }
        else {
            spaces.put(count, " ".repeat(count)) ?: ""
        }
    }
}
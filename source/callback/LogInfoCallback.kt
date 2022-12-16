package callback

import logging.LogInfo

interface LogInfoCallback {
    fun method(info: LogInfo)
}

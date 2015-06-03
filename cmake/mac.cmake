SET(SDK_MIN "10.7")
SET(SDK "10.7")
SET(DEV_SDK "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX${SDK}.sdk")
SET(DEV_SDK2 "/Developer/SDKs/MacOSX${SDK}.sdk")

IF (NOT EXISTS "${DEV_SDK}" AND NOT IS_DIRECTORY "${DEV_SDK}")
  IF (NOT EXISTS "${DEV_SDK2}" AND NOT IS_DIRECTORY "${DEV_SDK2}")
    MESSAGE("Could not find Mac OSX SDK at: ${DEV_SDK} or ${DEV_SDK2}")
    MESSAGE("Aborting!")
    RETURN()
  ELSE()
    SET(DEV_SDK ${DEV_SDK2})
  ENDIF()
ENDIF()

ADD_DEFINITIONS(
  -DMAC
  -DGCC_VISIBILITY
  -mmacosx-version-min=${SDK_MIN}
  )

SET(CMAKE_OSX_SYSROOT ${DEV_SDK})

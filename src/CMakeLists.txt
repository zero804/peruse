# A library for managing acbf (Advanced Comic Book Format) data, as defined by http://acbf.wikia.com/wiki/Specifications
add_subdirectory(acbf)

# Content provider Quick2 plugin
add_subdirectory(contentlist)

# Our Qt Quick plugin
add_subdirectory(qtquick)

# Comon helper library shared between app and creator.
add_subdirectory(common)

# Base application
add_subdirectory(app)

# Creator application
add_subdirectory(creator)

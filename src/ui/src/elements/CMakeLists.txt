target_sources(
    ${TARGET_NAME}
    PRIVATE
         XmlElement.cpp
         Group.cpp
         Linear.cpp
         Align.cpp
         Slot.cpp
         Clickable.cpp
         Text.cpp
         Ratio.cpp
         Rectangle.cpp
         EventBlocker.cpp
         Image.cpp
#        MonoImage.cpp
#        Switch.cpp
         ScrollArea.cpp
#        ScaleToFit.cpp
)

add_subdirectory( tests )

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
#        Image.cpp
#        MonoImage.cpp
#        Ratio.cpp
#        Rectangle.cpp
#        Switch.cpp
#        ScrollArea.cpp
#        PriorityListener.cpp
#        ScaleToFit.cpp
#        EventBlocker.cpp
)

add_subdirectory( tests )

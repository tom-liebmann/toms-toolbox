target_sources(
    ${TARGET_NAME}
    PRIVATE
         XmlElement.cpp
         Group.cpp
         Linear.cpp
         Align.cpp
#        Image.cpp
#        MonoImage.cpp
#        Label.cpp
#        Ratio.cpp
#        Rectangle.cpp
#        Clickable.cpp
#        Switch.cpp
#        ScrollArea.cpp
#        PriorityListener.cpp
#        ScaleToFit.cpp
#        EventBlocker.cpp
)

add_subdirectory( tests )

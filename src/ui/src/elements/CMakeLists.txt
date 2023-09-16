target_sources(
    ${TARGET_NAME}
    PRIVATE
         XmlElement.cpp
         Group.cpp
#        Center.cpp
#        Flex.cpp
#        Image.cpp
#        MonoImage.cpp
#        Label.cpp
#        Margin.cpp
#        Ratio.cpp
#        Rectangle.cpp
#        # RowLayout.cpp
#        Clickable.cpp
#        Switch.cpp
#        ScrollArea.cpp
#        PriorityListener.cpp
#        ScaleToFit.cpp
#        EventBlocker.cpp
)

add_subdirectory( tests )

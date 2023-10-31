#pragma once

#include <ttb/ui/XmlLoader.hpp>

#include <memory>


namespace ttb::ui
{
    class XmlFactoryBase
    {
    public:
        virtual ~XmlFactoryBase();

    private:
        virtual std::unique_ptr< Element >
            create( Root& root, rapidxml::xml_node<> const& node, XmlLoader& loader ) = 0;

        friend XmlLoader;
    };


    template < typename TType >
    class XmlFactory : public XmlFactoryBase
    {
    public:
        XmlFactory( std::string const& name );

    private:
        virtual std::unique_ptr< Element >
            create( Root& root, rapidxml::xml_node<> const& node, XmlLoader& loader ) override;
    };
}


namespace ttb::ui
{
    template < typename TType >
    XmlFactory< TType >::XmlFactory( std::string const& name )
    {
        XmlLoader::registerFactory( name, this );
    }

    template < typename TType >
    std::unique_ptr< Element > XmlFactory< TType >::create( Root& root,
                                                            rapidxml::xml_node<> const& node,
                                                            XmlLoader& loader )
    {
        auto element = std::make_unique< TType >( root );
        element->parseXml( node, loader );
        return element;
    }
}

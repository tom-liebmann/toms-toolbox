#include <ttb/ui/XmlLoader.hpp>

#include <ttb/ui/XmlFactory.hpp>
#include <ttb/ui/elements/XmlElement.hpp>


namespace
{
    std::unordered_map< std::string, ttb::ui::XmlFactoryBase* > elementFactories;
}


namespace ttb::ui
{
    Element* XmlLoader::loadElement( Framework& framework, rapidxml::xml_node<> const& node )
    {
        auto const factoryIter = elementFactories.find( node.name() );

        if( factoryIter == std::end( elementFactories ) )
        {
            std::cout << "Did not find " << node.name() << std::endl;
            return nullptr;
        }

        std::cout << "Loading: " << node.name() << std::endl;

        auto child = factoryIter->second->create( framework, node, *this );
        auto const childPtr = child.get();
        m_parent.m_elements.push_back( std::move( child ) );

        return childPtr;
    }

    XmlLoader::XmlLoader( XmlElement& parent ) : m_parent{ parent }
    {
    }


    void XmlLoader::registerFactory( std::string const& name, XmlFactoryBase* factory )
    {
        elementFactories.insert( { name, factory } );
    }
}

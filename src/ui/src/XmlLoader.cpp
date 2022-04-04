#include <ttb/ui/XmlLoader.hpp>

#include <ttb/ui/XmlFactory.hpp>
#include <ttb/ui/elements/XmlElement.hpp>

#include <utility>


namespace
{
    std::unordered_map< std::string, ttb::ui::XmlFactoryBase* >& elementFactories();
}


namespace ttb::ui
{
    Element* XmlLoader::loadElement( Framework& framework, rapidxml::xml_node<> const& node )
    {
        // Using string_view here is not possible due to unordered_map not allowing heterogenious
        // lookup
        auto const nodeName = std::string{ node.name(), node.name_size() };

        auto const factoryIter = elementFactories().find( nodeName );

        if( factoryIter == std::end( elementFactories() ) )
        {
            std::cout << "Did not find " << nodeName << std::endl;
            return nullptr;
        }

        std::cout << "Loading: " << nodeName << std::endl;

        auto child = factoryIter->second->create( framework, node, *this );
        auto const childPtr = child.get();
        m_parent.m_elements.push_back( std::move( child ) );

        if( auto id = attrValue( node, "id" ); id )
        {
            m_parent.m_elementIdIndex.insert( { std::string( id.value() ), childPtr } );
        }

        return childPtr;
    }

    std::optional< std::string_view > XmlLoader::attrValue( rapidxml::xml_node<> const& node,
                                                            std::string_view const& name )
    {
        for( auto attr = node.first_attribute(); attr; attr = attr->next_attribute() )
        {
            auto const attrName = std::string_view{ attr->name(), attr->name_size() };
            if( name == attrName )
            {
                return { std::string_view{ attr->value(), attr->value_size() } };
            }
        }

        return {};
    }

    XmlLoader::XmlLoader( XmlElement& parent ) : m_parent{ parent }
    {
    }


    void XmlLoader::registerFactory( std::string_view const& name, XmlFactoryBase* factory )
    {
        elementFactories().insert( { std::string( name ), factory } );
    }
}


namespace
{
    std::unordered_map< std::string, ttb::ui::XmlFactoryBase* >& elementFactories()
    {
        static auto factories = std::unordered_map< std::string, ttb::ui::XmlFactoryBase* >{};
        return factories;
    }
}

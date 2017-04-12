//#pragma once
//
//namespace ttb
//{
//    class EventSource
//    {
//    public:
//        virtual ~EventSource();
//
//        void addListener( std::shared_ptr< EventListener > const& listener );
//        void removeListener( std::shared_ptr< EventListener > const& listener );
//
//        virtual void process() = 0;
//
//    protected:
//        void handleEvent( Event const& event ) const;
//
//    private:
//        std::vector< std::shared_ptr< EventListener > > m_listener;
//    };
//}

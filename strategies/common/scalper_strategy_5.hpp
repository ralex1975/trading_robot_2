//#pragma once

//#include <atrade/atrade.hpp>
//#include <functions/functions.hpp>
//#include <strategies/common/simple_closing_2.hpp>
//#include <strategies/common/simple_closing_3.hpp>
//#include <indicators/common/price_indicator.hpp>
//#include <indicators/common/price_indicator_2.hpp>
//#include <indicators/common/market_order_indicator.hpp>
//#include <indicators/common/step_indicator.hpp>
//#include <indicators/common/volume_indicator.hpp>
//#include <iostream>

//class ScalperStrategy5 : public atrade::async2::IStrategy
//{
//public:
//    typedef std::shared_ptr<atrade::async2::BooleanIndicator> IndicatorPtr;

//    ScalperStrategy5(boost::asio::io_service* ios, atrade::async2::TradingContext* ctx, int64_t isin_id) :
//        m_ios(ios),
//        m_ctx(ctx),
//        m_isin_id(isin_id),
//        m_profit_trades(0),
//        m_loss_trades(0),
//        break_1(0),
//        break_2(0),
//        break_3(0),
//        m_executed(false),
//        m_max_price(0)
//    {
//    }

//    ScalperStrategy5(boost::asio::io_service& ios, atrade::async2::TradingContext& ctx, int64_t isin_id) :
//        ScalperStrategy5(&ios, &ctx, isin_id)
//    {
//    }

//    ScalperStrategy5(int64_t isin_id) :
//        ScalperStrategy5(nullptr, nullptr, isin_id)
//    {
//    }

//    ~ScalperStrategy5()
//    {
//        if (!m_executed)
//        {
//            return;
//        }
//        if (m_profits.size() > 0 || m_losses.size() > 0)
//        {
//            std::cout << "profit trades=" << m_profits.size()
//                      << " loss trades=" << m_losses.size()
//                      << " rel=" << static_cast<double>(m_losses.size())*100/(m_losses.size()+m_profits.size()) << "%" << "\n";
//            std::cout << "avg_profit=" << functions::mean(m_profits, m_profits.size())
//                      << " avg_loss=" << functions::mean(m_losses, m_losses.size()) << "\n";
//            std::cout << "profit=" << atrade::calc_profit(m_orders) << "\n";
//        }
//        std::cout << "break_1=" << break_1 << " break_2=" << break_2
//                  << " break_3=" << break_3 << "\n";
//        std::cout << "max_price=" << m_max_price << "\n";
//    }

//    void operator() (boost::asio::yield_context yield)
//    {
//        (*this)(*m_ios, *m_ctx, yield);
//    }

//    void operator() (boost::asio::io_service& ios, atrade::async2::TradingContext& ctx, atrade::asio::yield_context yield)
//    {
//        using namespace atrade;
//        using namespace functions;

//        using asio::io_service;
//        using async2::TradingContext;
//        using asio::yield_context;
//        using async2::Timer;
//        using Watcher = async2::Watcher<async2::BooleanIndicator>;

//        using asio::io_service;
//        using async2::TradingContext;
//        using asio::yield_context;
//        using async2::Timer;
//        using Watcher = async2::Watcher<async2::BooleanIndicator>;

//        using TimerPtr = std::unique_ptr<Timer>;
//        using WatcherPtr = std::unique_ptr<Watcher>;

//        m_executed = true;

//        atrade::Security security = async2::get_security(ios, ctx, m_isin_id, yield);
//        double step = security.step();

//        int i = 0;
//        int loss_trades = 0;
//        int mul = 1;

//        m_order_subscription = async2::subscribe_order(ios, [&](const Order& order) {
//            if (order.isin_id == m_isin_id)
//            {
//                m_orders.push_back(order);
//            }
//        });

//        pt::time_duration td = pt::seconds(100);
//        DealType deal_type = DealType::Undefined;
//        DealType g_deal_type = DealType::Undefined;
//        bool is_pos = false;

//        auto _ts = async2::subscribe_tick(ios, [&](const atrade::Deal tick) {
//            m_max_price = std::max(m_max_price, tick.price);
//        });

////        std::unordered_map<int64_t, std::vector<Quote>> quotes;
////        auto ss1 = async2::subscribe_stock(ios, [&](int64_t isin_id, const std::vector<Quote>& stocks) {
////                quotes[isin_id] = stocks;
////        });
////        auto ss2 = async2::subscribe_stock(ios, [&](int64_t isin_id, const std::vector<Quote>& stocks) {
////                quotes[isin_id] = stocks;
////        });

//        DealType glob_dir = DealType::Undefined;

//        MarketOrderIndicator gmoi(ios, m_isin_id, pt::seconds(90), 2);

//        auto glob_dir_subscr = gmoi.subscribe([&](DealType type) {
//                glob_dir = type;
//        });

//        auto print_dir = [&](DealType type) {
//            switch (type)
//            {
//            case DealType::Undefined:
//                return "undefined";
//            case DealType::Buy:
//                return "buy";
//            case DealType::Sell:
//                return "sell";
//            default:
//                return "undefined";
//            }
//        };

//        async2::Spawner spawner(ios, ctx);
//        while (true)
//        {
//            async2::Market market(ios, spawner.ctx());

//            double cprice = center_price(ios, m_isin_id);

//            deal_type = DealType::Undefined;

//            PriceIndicator2 moi(ios, m_isin_id, 30, pt::seconds(40));
//            async2::Watcher<PriceIndicator2> mo_watcher(ios, ctx, &moi);
//            mo_watcher.async_wait(true, yield);

//            //std::cout << "hello\n";

//            boost::system::error_code errc;

//            OrderDir order_dir;

//            OrderBook orderbook(ios, m_isin_id);

////            if (orderbook.bids(10) >= 2*orderbook.asks(15))
////            {
////                deal_type = DealType::Buy;
////                order_dir = OrderDir::Bid;
////            }
////            else if (orderbook.bids(15)*2 <= orderbook.asks(10))
////            {
////                deal_type = DealType::Sell;
////                order_dir = OrderDir::Ask;
////            }
////            else
////            {
////                std::cout << "continue " << orderbook.bids(15)
////                          << " " << orderbook.asks(15) << "\n";
////                continue;
////            }

//            if (orderbook.bids(15) >= orderbook.asks(15))
//            {
//                deal_type = DealType::Buy;
//                order_dir = OrderDir::Bid;
//            }
//            else if (orderbook.bids(15) <= orderbook.asks(15))
//            {
//                deal_type = DealType::Sell;
//                order_dir = OrderDir::Ask;
//            }
//            else
//            {
//                std::cout << "continue " << orderbook.bids(15)
//                          << " " << orderbook.asks(15) << "\n";
//                continue;
//            }

//            double orel;

//            OrderBook orderbook2(ios, m_isin_id);

//            int dist = 10;

//            OrderBook orderbook3(ios, m_isin_id);

//            Order order;
//            int order_id;

//            std::vector<Order> __orders;
//            Order _order;

//            auto t1 = async2::current_datetime(ios);

//            order = Order(m_isin_id,
//                          order_dir,
//                          (order_dir == OrderDir::Bid) ?
//                              orderbook3.best_bid() + 1*step :
//                              orderbook3.best_ask() - 1*step,
//                          1);
//            order_id = market.async_add_order(order, yield);
//            async2::WaitDeal wait_deal_1(ios, ctx, order_id);
//            __orders = wait_deal_1.async_wait(true, pt::seconds(10), yield[errc]);
//            if (errc)
//            {
//                std::cout << "shit\n";
//                spawner.spawn(SimpleClosing3(market.market(), m_isin_id));
//                spawner.async_join(yield);
//                continue;
//            }
//            _order = __orders.front();

//            if (order_dir == OrderDir::Bid)
//            {
//                orel = double(orderbook2.bids(dist))/orderbook2.asks(dist);
//            }
//            else
//            {
//                orel = double(orderbook2.asks(dist))/orderbook2.bids(dist);
//            }

//            int64_t did1 = _order.deal_id, did2;
//            double dp1 = _order.deal_price, dp2;

////            std::cout << " deal_price = " << _order.deal_price;

////            MarketOrderIndicator market_order_indicator_2(ios, m_isin_id, 10, pt::seconds(5));
////            async2::Watcher<MarketOrderIndicator> mo_watcher_2(ios, ctx, &market_order_indicator_2);
////            mo_watcher_2.async_wait_not(!deal_type, pt::seconds(10), yield[errc]);

////            Timer timer(ios, ctx, pt::seconds(20));
////            timer.async_wait(yield);

//            const int max_volume = 150;

//            order.dir = !order.dir;
//            order.type = atrade::OrderType::Limit;
//            order.price = find_price(ios, m_isin_id, order.dir, max_volume);

//            int delta = std::abs(std::round( (order.price - find_price(ios, m_isin_id, order.dir, 300))/step ));

////            order.price = (order.dir == OrderDir::Bid) ?
////                        _order.deal_price-5*step :
////                        _order.deal_price+5*step;
//            order_id = market.async_add_order(order, yield);

////            PriceIndicator price_indicator(ios,
////                                           m_isin_id,
////                                           !deal_type,
////                                           (deal_type == DealType::Buy) ?
////                                               _order.deal_price-5*step :
////                                               _order.deal_price+5*step);

////            spawner.spawn([&](io_service& ios, TradingContext& ctx, yield_context yield) {
////                async2::Watcher<PriceIndicator> watcher(ios, ctx, &price_indicator);
////                watcher.async_wait(yield[errc]);
////                spawner.reset();
////            });

//            bool is_init = false;
//            double bad_price = 0;

//            auto _ts = async2::subscribe_tick(ios, [&](const atrade::Deal tick) {
//                if (!bad_price)
//                {
//                    bad_price = tick.price;
//                    is_init = true;
//                }
//                if (order.dir == OrderDir::Bid && tick.type == DealType::Sell)
//                {
//                    bad_price = std::max(bad_price, tick.price);
//                }
//                else if (order.dir == OrderDir::Ask && tick.type == DealType::Buy)
//                {
//                    bad_price = std::min(bad_price, tick.price);
//                }
//            });

//            bool _opp = false;

//            MarketOrderIndicator market_order_indicator2(ios, m_isin_id, 400, pt::seconds(5));

////            auto _mo_subscr = market_order_indicator2.subscribe([&](DealType type) {
////                    if (type == !deal_type)
////                    {
////                        _opp = true;
////                    }
////            });

//            pt::ptime t2;
//            std::vector<Order> res;

////            spawner.spawn([&](io_service& ios, TradingContext& ctx, yield_context yield) {
////                async2::Watcher<MarketOrderIndicator> mo_watcher(ios, ctx, &market_order_indicator2);
////                g_deal_type = mo_watcher.async_wait(!deal_type, yield);
////                spawner.reset();
////                t2 = async2::current_datetime(ios);
////            });

//            auto async_wait_deal = [&]() {
//                spawner.spawn([&](TradingContext& ctx, yield_context yield) {
//                    try
//                    {
//                        async2::WaitDeal wait_deal_2(ios, ctx, order_id);
//                        std::cout << "wait_deal start time:" << async2::current_datetime(ios) << "\n";
////                        res = wait_deal_2.async_wait(true, pt::seconds(90), yield);
//                        res = wait_deal_2.async_wait(true, yield);
//                        spawner.reset();
//                    }
//                    catch(const boost::system::system_error& e)
//                    {
//                        if (e.code() == async2::Errc::MakeDealTimeout)
//                        {
////                            std::cout << "wait deal reset:" << async2::current_datetime(ios) << "\n";
////                            spawner.reset();
//                        }
//                    }
////                    catch(...)
////                    {
////                    }
//                }, [&](boost::system::error_code errc) {
//                    if (!res.empty())
//                    {
//                        did2 = res.front().deal_id;
//                        dp2 = res.front().deal_price;
//                    }
//                });
//            };

//            async_wait_deal();

//            int move_count = 0;

////            spawner.spawn([&](io_service& ios, TradingContext& ctx, yield_context yield) {
////                for (;;move_count++)
////                {
////                    VolumeIndicator vindicator(ios, m_isin_id, order.dir, order.price, max_volume);
////                    async2::Watcher<VolumeIndicator> watcher(ios, ctx, &vindicator);
////                    bool res = watcher.async_wait(yield);
////                    if (res)
////                    {
////                        order.price = find_price(ios, m_isin_id, order.dir, max_volume);
//////                        std::cout << "price=" << order.price << "\n";
////                        order_id = market.async_move_order(order_id, order.price, yield);
////                        async_wait_deal();
////                    }
////                }
////            });

//            DealType _type = ((order.dir == OrderDir::Bid) ?
//                                  DealType::Sell : DealType::Buy);
//            double _price = ((order.dir == OrderDir::Bid) ?
//                                 _order.deal_price+25*step :
//                                 _order.deal_price-25*step);
//            PriceIndicator price_indicator(ios, m_isin_id, _type, _price);

////            spawner.spawn([&](io_service& ios, TradingContext& ctx, yield_context yield) {
////                Timer timer(ios, ctx, pt::seconds(5));
////                timer.async_wait(yield);
////                spawner.reset();
////            });

//            bool pri = false;

//            spawner.spawn([&](TradingContext& ctx, yield_context yield) {
//                for(;;)
//                {
//                    async2::Watcher<PriceIndicator> price_watcher(ios, ctx, &price_indicator);
//                    price_watcher.async_wait(true, yield);
//                    Timer timer(ios, ctx, pt::seconds(2));
//                    timer.async_wait(yield);
//                    if (price_indicator.value())
//                    {
//                        break;
//                    }
//                }
//                std::cout << "price_indicator reset:" << async2::current_datetime(ios) << "\n";
//                pri = true;
//                spawner.reset();
//            });

//            spawner.async_join(yield);

//            t2 = async2::current_datetime(ios);

////            std::cout << "move_count=" << move_count << "\n";

////            spawner.spawn(SimpleClosing2(m_isin_id));
////            spawner.async_join(yield);

//            spawner.reset();
//            spawner.spawn(SimpleClosing3(market.market(), m_isin_id));
//            spawner.async_join(yield);

////            if (errc)
////            {
////                spawner.reset();
////                spawner.spawn(SimpleClosing2(m_isin_id));
////                spawner.async_join(yield);
////            }

//            auto _orders = market.market().history().get_orders_by_isin_id(m_isin_id);

//            ++i;
//            double cur_profit = atrade::calc_profit(_orders);
//            if (cur_profit > 0)
//            {
//                is_pos = true;
//                td = t2 - t1;
//                m_profits.push_back(cur_profit);
////                if (cur_profit > 10)
////                {
////                    std::cout << "BIG PROFIT: ";
////                    for (auto& order : _orders)
////                    {
////                        std::cout << order << " ";
////                    }
////                    std::cout << "\n";
////                }
//            }
//            else
//            {
//                is_pos = false;
//                td = pt::seconds(100);
//                m_losses.push_back(cur_profit);
//            }

//            std::cout << t1 << " "
//                      << ((deal_type == DealType::Buy) ? "buy" : "sell")
//                      << " cur_profit=" << cur_profit
//                      << " orders=" << _orders.size()
//                      << " glob=" << print_dir(glob_dir)
////                      << " orel=" << orel
////                      << " did1=" << did1
////                      << " bp1=" << dp1
////                      << " did2=" << did2
////                      << " bp2=" << dp2
////                      << " init=" << is_init
////                      << " bad_delta=" << std::abs(bad_price - order.price)
////                      << " opp=" << _opp
//                      << " pri=" << pri
//                      << " delta=" << delta
//                      << " " << t2 - t1 <<"\n";
//        }
//    }

//    void run(boost::asio::io_service& ios, atrade::async2::TradingContext& ctx, boost::asio::yield_context yield)
//    {
//        (*this)(ios, ctx, yield);
//    }

//private:
//    boost::asio::io_service* m_ios;
//    atrade::async2::TradingContext* m_ctx;
//    int64_t m_isin_id;
//    int m_profit_trades;
//    int m_loss_trades;
//    std::vector<double> m_profits;
//    std::vector<double> m_losses;
//    std::vector<atrade::Order> m_orders;
//    atrade::async2::Subscription<atrade::OrderHandler> m_order_subscription;
//    int break_1;
//    int break_2;
//    int break_3;
//    bool m_executed;
//    double m_max_price;
//};



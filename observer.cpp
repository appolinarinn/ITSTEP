#include <iostream>
#include <memory>
#include <string>

using namespace std;

class RouteStrategy {
public:
    virtual void buildRoute(const string& A, const string& B) const = 0;
    virtual ~RouteStrategy() {}
};

class RoadStrategy : public RouteStrategy {
public:
    void buildRoute(const string& A, const string& B) const override {
        cout << "Прокладываю автомобильный маршрут из " << A << " в " << B << " по дорогам.\n";
    }
};

class WalkingStrategy : public RouteStrategy {
public:
    void buildRoute(const string& A, const string& B) const override {
        cout << "Прокладываю маршрут для пешехода из " << A << " в " << B << " по тротуарам и паркам.\n";
    }
};

class PublicTransportStrategy : public RouteStrategy {
public:
    void buildRoute(const string& A, const string& B) const override {
        cout << "Прокладываю маршрут на общественном транспорте из " << A << " в " << B << ".\n";
    }
};

class Navigator {
private:
    shared_ptr<RouteStrategy> strategy;

public:
    void setStrategy(shared_ptr<RouteStrategy> newStrategy) {
        strategy = newStrategy;
    }

    void buildRoute(const string& A, const string& B) const {
        if (strategy) {
            strategy->buildRoute(A, B);
        }
        else {
            cout << "Стратегия маршрута не выбрана.\n";
        }
    }
};

int main() {
    Navigator nav;

    string start = "Центр";
    string end = "Аэропорт";

    nav.setStrategy(make_shared<RoadStrategy>());
    nav.buildRoute(start, end);

    nav.setStrategy(make_shared<WalkingStrategy>());
    nav.buildRoute(start, end);

    nav.setStrategy(make_shared<PublicTransportStrategy>());
    nav.buildRoute(start, end);

    return 0;
}

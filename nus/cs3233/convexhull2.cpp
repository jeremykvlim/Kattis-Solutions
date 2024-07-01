#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}

    auto operator<(Point<T> &p) const {
        return x != p.x ? x < p.x : y < p.y;
    }

    auto operator==(Point<T> &p) const {
        return x == p.x && y == p.y;
    }
};

template <typename T>
double cross(Point<T> a, Point<T> b, Point<T> c) {
    return (double) (c.x - a.x) * (double) (b.y - a.y) - (double) (c.y - a.y) * (double) (b.x - a.x);
}

template <typename T>
deque<Point<T>> monotone(vector<Point<T>> &points) {
    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());

    if (points.size() < 3) return deque<Point<T>>(points.begin(), points.end());
        
    deque<Point<T>> convex_hull;
    for (auto p : points) {
        while (convex_hull.size() > 1 && cross(convex_hull[1], convex_hull[0], p) > 0) convex_hull.pop_front();
        convex_hull.emplace_front(p);
    }

    int s = convex_hull.size();
    points.pop_back();
    reverse(points.begin(), points.end());
    for (auto p : points) {
        while (convex_hull.size() > s && cross(convex_hull[1], convex_hull[0], p) > 0) convex_hull.pop_front();
        convex_hull.emplace_front(p);
    }

    convex_hull.pop_front();
    return convex_hull;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Point<int>> points;
    while (n--) {
        int x, y;
        char c;
        cin >> x >> y >> c;

        if (c == 'Y') points.emplace_back(x, y);
    }

    auto convex_hull = monotone(points);
    reverse(convex_hull.begin(), convex_hull.end());
    cout << convex_hull.size() << "\n";
    for (auto [x, y] : convex_hull) cout << x << " " << y << "\n";
}

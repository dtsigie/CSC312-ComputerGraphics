#ifndef _HIT_RECORD_H_
#define _HIT_RECORD_H_

#include "csc321.h"
#include "Point3.h"
#include <list>
#include <iostream>

// use this class to store your intersections

class HitRecord {
public:
    HitRecord() : hits() {}
    HitRecord(const HitRecord& hr) : hits(hr.hits) {}
    ~HitRecord() {}
  
    HitRecord& operator=(const HitRecord& hr) {
        if (this != &hr)
            hits = hr.hits;
        return *this;
    }
    
    void addHit(double t, double u, double v, Point3 p, Vector3 n) {
        // add a single hit to the back of the hit list
        Hit h;
        h.t = t;
        h.u = u;
        h.v = v;
        h.p = p;
        h.n = n;
        hits.push_back(h);
    }
    
    void addHits(HitRecord& hr) {
        // add all the hits in the given hit record to this one
        hits.merge(hr.hits);
    }
    
    void sortHits() {
        // sort hits by their T values
        hits.sort();
    }
    
    bool getFirstHit(double& t, double& u, double& v, Point3& p, Vector3& n) {
        // return the first hit on the list if there is one
        // you must call sortHits if you want the closest hit
        // (unless you can guarentee the order in which hits
        // are added to the list)
        if (hits.size() == 0)
            return false;
        Hit& h = hits.front();
        t = h.t;
        u = h.u;
        v = h.v;
        p = h.p;
        n = h.n;
        return true;
    }
    
    void removeFirstHit() {
        // removes the first hit if there is one, otherwise
        // just returns without doing anything
        if (hits.size() > 0)
            hits.pop_front();
    }
    
private:
    class Hit {
    public:
        Hit() {}
        Hit(const Hit& h) : t(h.t), u(h.u), v(h.v), p(h.p), n(h.n) {}
        ~Hit() {}
        
        Hit& operator=(const Hit& h) {
            if (this != &h) {
                t = h.t;
                u = h.u;
                v = h.v;
                p = h.p;
                n = h.n;
            }
            return *this;
        }
        
        bool operator<(Hit& right) {
            return t < right.t;
        }
        
        double t, u, v;
        Point3 p;
        Vector3 n;
    };
    
    friend std::ostream& operator<<(std::ostream& os, HitRecord& hr);
    
    std::list<Hit> hits;
};

inline std::ostream& operator<<(std::ostream& os, HitRecord& hr) {
    // print out all hits
    os << "Hits: " << hr.hits.size();
    for (std::list<HitRecord::Hit>::iterator it = hr.hits.begin(); it != hr.hits.end(); it++) {
        HitRecord::Hit& h = *it;
        os << " |";
        os << " " << h.t;
     //   os << " " << h.u;
     //   os << " " << h.v;
        os << " " << h.p;
        os << " " << h.n;
    }
    return os;
}

#endif /* _HIT_RECORD_H_ */

#include "Contact.hpp"

Contact::Contact(   glm::vec3 contactPoint,
                    glm::vec3 contactNormal,
                    float penetration) : \
                    contactPoint(contactPoint),
                    contactNormal(contactNormal),
                    penetration(penetration)
{

}
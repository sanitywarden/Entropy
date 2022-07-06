#ifndef _QUEST_HPP_
#define _QUEST_HPP_

#include <string>

namespace iso {
    class Quest {
        public:
            std::string quest_title; 
            std::string quest_description;
            std::string quest_code_id;
            std::string quest_icon;

        public:
            Quest();
            ~Quest();

            virtual bool isFinishConditionSatisfied() const { return false; }
            virtual void onQuestFinish() { return; }
    };
}

#endif
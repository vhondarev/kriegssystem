#ifndef KS_MESSAGES_H
#define KS_MESSAGES_H

#ifdef DEBUG
#define PRINT(...) printf(__VA_ARGS__)
#else
#define PRINT(...) ((void)0)
#endif

#define MSG_ERR_MEMORY_FAIL "Memory allocation failed"

#define MSG_ERR_PARSE_LIMIT "Too many teams"
#define MSG_ERR_PARSE_SETTLER "Team settler argument absent"
#define MSG_ERR_PARSE_TYPE_OR_QUANTITY "Invalid vessel type or vessels number"

#define MSG_ERR_VESSEL_TYPE "Invalid vessel type"
#define MSG_ERR_VESSEL_NULL_INIT "Initialize vessel state on NULL"
#define MSG_ERR_FLEET_INIT "Failed init fleet"
#define MSG_ERR_FLEET_EMPTY "Failed init fleet, 0 vessels parsed"
#define MSG_ERR_VESSEL_LIMIT "Reached maximum vessel count"

#define MSG_INVALID_ARGUMENTS "Invalid arguments\n"
#define MSG_EXE_HINT "./naval team [t]:[n] ... team [t]:[n]...\n"
#define MSG_EXE_TIME_REPORT "Execution time: %f seconds\n"

#define MSG_ATTACKER "Attacker"
#define MSG_DEFENDER "Defender"

#define MSG_ROUND_INFO "\nRound %d is starting.\n"
#define MSG_PHASE_SHOOTING "\n*Phase*: Shooting\n"
#define MSG_PHASE_CASUALTIES "\n*Phase*: Casualties\n"
#define MSG_PHASE_BOARDING "\n*Phase*: Boarding\n"
#define MSG_END_ROUND_INFO "\nRound %d finished:\n"
#define MSG_LEFT_VESSELS "%s has %zu vessels left\n"

#define MSG_CURRENT_TURN "\n____%s Turn____\n"
#define MSG_TARGETING_RESULT "%s _vs_ %s\n"
#define MSG_SHOOTING_REPORT "%d rounds executed: %d effective, %d damage\n"
#define MSG_VESSELS_COLLISION "__Vessels Collision__\n"
#define MSG_COLLISION_REPORT "%s lost %d hull points, going from %d down to %d\n"
#define MSG_VESSELS_BOARDING_FIGHT "__Crew Fight__\n"
#define MSG_BOARDING_FIGHT_REPORT "%s lost %d crew members, dropping from %d down to %d\n"
#define MSG_VESSEL_STATUS "%s status: %d hull remaining, %d crew members alive\n"
#define MSG_VESSEL_SUNKED "%s has sunk\n"
#define MSG_VESSEL_CREW_SURRENDERED "The crew of the %s %s has surrendered\n"
#define MSG_BOARDING_TRY "__Try to board__\n"

#define MSG_END_BATTLE_INFO "\nBattle ended in %d rounds\n"
#define MSG_ATTACKER_WON "Attacker won!\n"
#define MSG_DEFENDER_WON "Defender won!\n"
#define MSG_STALEMATE_RESULT "Battle ended in stalemate. Both fleets still have vessels.\n"
#define MSG_MUTUAL_DESTRUCTION_RESULT "Mutual destruction! Both fleets have been eliminated.\n"

#endif

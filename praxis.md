### Thoughts on praxis

Praxis is formed around objects of practices. These practices have sub-states which activates (allow for) actions - which again is limited by their precondition and some conditioning in post conditions.

The format is:

```
process.<name>.<parameter>(agent) // see if it matches and defines aliases

	start
		// initialising procedures

		insert <exclusion logic>, 
		delete <exclusion logic>, 
		call <another label>, 
		text "<text with alias refences>" to <channel in the form of actor> ,
		if then else end - clauses
		calculus on numeric variables (aliases)

		reference to <Self>

	state!<key>.<key>

		action "Label category | label sub category" // see if it matches and defines aliases

			precondtions

				<declarative> // see if it matches and defines aliases

				example: 

					brown.at!Location // here we automatically match in state DB and set Location equal to actal sub key to brown.at!??

				reference to <Parent>

			postconditions

				<imperative commands>

				insert <exclusion logic>, 
				delete <exclusion logic>, 
				call <another label>, 
				text "<text with alias refences>" to <channel in the form of actor> ,
				if then else end - clauses
				calculus on numeric variables (aliases)

				reference to <Parent>

	<function label>.<parameter>(agent)

				<imperative commands>
``` 

Actors evaluates all affordances (available actions) via tagged (or more precis scored) logic sentences. So again for an actor called brown:

```
	Other.sex!male and 
	Other.class!upper and
	is_displeased_with.Other.brown
```

Is really stating, that Brown likes other upperclass male being displeased with him. So if his affordances includes _provoke_ and he is in room with upperclass males on whom he can use his action _provoke_, he can score say 20 points per upperclass male being insulted.

The evaluation must somehow be connected to the agent. Thus the sequence of the algorithm must be something like 
```
 1) For all active practices (i.e. practices with parameters matching world state database) 
 	i) for relevant states
 		  a) for all acions
 		  b) verify preconditions and if they check out
 		  	 it is important to note that Actor controls many preconditions
 		  	 How is Actor controlled? 
 		  c) simulate implications
 		  d) do some evaluation based on the agents prefences
 		  e) set score for all actions simulated - these are allowances
 		  f) choose either the maximum or maybe some distributed randomeness

```
Actor variable seem to control turn taking in some of the examples. We may expect that everyone is taking turns (and not only in the episode - things may happen outside of player scope?). Now and then when a character (not necessarily the player) is confronted - this character is pushed ahead in queue. 

If player/character passes his turn (not likely for NPC being norm abiding) - the next character in line may react. Player can choose if they want to ignore their turn by pressen __more__ button or respond via __act__ button.  


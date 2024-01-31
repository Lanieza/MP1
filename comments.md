# Comments

## Functionality
- forgot to include the following as delimiter (note: they are considered as white spaces)
	- `\t`
- \[tokenize()\] line 155, no case of lastToken. SEMICOLON is already parsed within the while loop
- I think check_Variable() and/or getType should check if it's a possible value (?)
- check_Variable also doesn't check for correct sequences
- a token with a format like "=23" is parsed as UNEXPECTED. I think there should be another token type for classifying this. Or at least same functionality as if it were "= 23"

## Performance
- \[check_Variable()\] line 89, better to use switch-case here since it's cleaner (in my opinion) and way faster than if-else branches
	- For readability, my rule of thumb is usually: if else-if when cases are less than 3 since it's slightly cleaner. But when there are 3 or more cases + a default, then use switch-case
- \[getType()\] line 47, I think it's much better to place this (isIdentifier()) as the last else-if statement before else because this has an extra overhead of a function call whenever the code passes through this. Better to check all other cases first since this is relatively expensive. Though this changes when analyzing how common the cases are to appear. But that's another story of compiler code analysis advanced stuff thingy hahaha. But the general rule follows in my first assumption about placing this last.

## Style/Consistency (only affects maintainability and readability)
- `using namespace std;` isn't good practice, but it's fine for now
- inconsistency: usage of both `string` and `std::string`
- have a more consistent naming convention:
	- identifiers used were sometimes camelCase, snake_case, or a combination of both (check_Variable)
	- much better to use only one
	- There are many style guides out there that touches on naming conventions. Here's [Google's](https://google.github.io/styleguide/cppguide.html)
	- But that's overkill. For starters, I'd suggest snake_case since they're easier to read (all lowercases with words separated by _)
- \[isIdentifier\] line 27, much better to use a [Guard Clause](https://deviq.com/design-patterns/guard-clause)
Ex: 
```c++
// Haven't tested this, but you can also verify using DeMorgan's law. Basically flipped your original condition to the opposite, ie, applying NOT
if (str.empty() || !(str[0] == '_' && isalpha(str[0]))) 
	return false

for (char c: str.substr(1)) {
	...
}
```

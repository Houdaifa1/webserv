#!/bin/bash

# This script is a "Guess the Number" game.
# GET starts a new game.
# POST processes a guess.

# --- Function to print the main game form ---
# $1 = Message (e.g., "Too low!", "Guess a number...")
# $2 = Secret Number (to hide in the form)
# $3 = Guess Count (to hide in the form)
print_game_form() {
    local MESSAGE="$1"
    local SECRET_NUM="$2"
    local GUESS_COUNT="$3"

    cat << EOF
    <h1>Guess The Number</h1>
    <p>Current Guesses: <span class="output">$GUESS_COUNT</span></p>
    <h2>$MESSAGE</h2>
    
    <form method="POST" action="guess_game.sh">
        <input type="hidden" name="secret" value="$SECRET_NUM">
        <input type="hidden" name="guesses" value="$GUESS_COUNT">
        
        <label for="guess">Your Guess:</label>
        <input type="number" id="guess" name="guess" min="1" max="100" required>
        <button type="submit">Submit Guess</button>
    </form>
EOF
}

# --- Function to print the win page ---
# $1 = Secret Number
# $2 = Final Guess Count
print_win_page() {
    local SECRET_NUM="$1"
    local FINAL_COUNT="$2"

    cat << EOF
    <h1>You Guessed It!</h1>
    <pre>
<span class="prompt">$</span> <span class="output">Target acquired: $SECRET_NUM</span>
<span class="prompt">$</span> <span class="output">Attempts logged: $FINAL_COUNT</span>
<span class="prompt">$</span> <span class="output">System... compromised.</span>
    </pre>
    <a href="guess_game.sh" class="play-again">Play Again</a>
EOF
}


# --- Common CSS and HTML header ---
cat << EOF
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>CGI Guess Game</title>
    <style>
        body {
            background-color: #000000;
            color: #39ff14;
            font-family: 'Courier New', Courier, monospace;
            padding: 2rem;
            text-align: center;
        }
        h1, h2 {
            text-shadow: 0 0 5px #39ff14, 0 0 10px #39ff14;
        }
        h1 {
            border-bottom: 2px solid #39ff14;
            padding-bottom: 5px;
        }
        pre {
            background-color: #0a0a0a;
            border: 1px solid #39ff14;
            padding: 1rem;
            border-radius: 8px;
            box-shadow: 0 0 10px #39ff14;
            white-space: pre-wrap;
            word-wrap: break-word;
            text-align: left;
            max-width: 500px;
            margin: 2rem auto;
        }
        form {
            background-color: #0a0a0a;
            border: 1px solid #39ff14;
            border-radius: 8px;
            padding: 2rem;
            max-width: 400px;
            margin: 1rem auto;
        }
        input, button {
            background-color: #111;
            border: 1px solid #39ff14;
            color: #39ff14;
            padding: 0.75rem;
            border-radius: 4px;
            font-family: 'Courier New', Courier, monospace;
            width: 90%;
            margin-bottom: 1rem;
        }
        button, .play-again {
            background-color: #39ff14;
            color: #000;
            cursor: pointer;
            font-weight: bold;
            text-decoration: none;
            display: inline-block;
            padding: 0.75rem 1.5rem;
        }
        .prompt { color: #ffffff; }
        .output { color: #39ff14; font-weight: bold; }
    </style>
</head>
<body>
EOF

# --- Main Logic ---

if [ "$REQUEST_METHOD" = "POST" ]; then
    # --- POST RESPONSE (Process a guess) ---
    
    # Read raw form data
    read POST_DATA
    
    # Parse the data (simple parser)
    USER_GUESS=$(echo "$POST_DATA" | tr '&' '\n' | grep '^guess=' | cut -d'=' -f2)
    SECRET_NUM=$(echo "$POST_DATA" | tr '&' '\n' | grep '^secret=' | cut -d'=' -f2)
    GUESS_COUNT=$(echo "$POST_DATA" | tr '&' '\n' | grep '^guesses=' | cut -d'=' -f2)

    # Increment guess count
    GUESS_COUNT=$((GUESS_COUNT + 1))
    MESSAGE=""

    # Check for non-numeric or empty guess
    if ! [[ "$USER_GUESS" =~ ^[0-9]+$ ]]; then
        MESSAGE="Invalid guess. Enter a number."
        # Call form again with same secret and new count
        print_game_form "$MESSAGE" "$SECRET_NUM" "$GUESS_COUNT"
        
    elif [ "$USER_GUESS" -lt "$SECRET_NUM" ]; then
        MESSAGE="Target is higher. (Too Low)"
        print_game_form "$MESSAGE" "$SECRET_NUM" "$GUESS_COUNT"
        
    elif [ "$USER_GUESS" -gt "$SECRET_NUM" ]; then
        MESSAGE="Target is lower. (Too High)"
        print_game_form "$MESSAGE" "$SECRET_NUM" "$GUESS_COUNT"
        
    else
        # Correct guess!
        print_win_page "$SECRET_NUM" "$GUESS_COUNT"
    fi

elif [ "$REQUEST_METHOD" = "GET" ]; then
    # --- GET RESPONSE (Start new game) ---
    
    # Generate random number between 1 and 100
    SECRET_NUM=$(( ( RANDOM % 100 ) + 1 ))
    MESSAGE="New target acquired. Guess (1-100)."
    GUESS_COUNT=0
    
    print_game_form "$MESSAGE" "$SECRET_NUM" "$GUESS_COUNT"
fi

# --- Common HTML footer ---
cat << EOF
</body>
</html>
EOF
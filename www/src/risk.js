      var myScroll;

      var canClick = true;
      var player;

      var mode = "picker";

      function loaded() {
        myScroll = new IScroll('#wrapper', {
          scrollX: true,
          scrollY: false,
          mousewheel: true
        });
        myScroll.on("scrollStart", function() { 
          canClick = false;
        });
        myScroll.on("scrollEnd", function() {
          canClick = true;
        });
        myScroll.on("tap", function() {
          canClick = true;
        });
        // Update cards every 100ms
        setInterval(function() { update(); }, 100);
        setInterval(function() { if (player !== undefined) { updateStatistics(); } }, 1000);
      }

      var numCards = 0;

      var cardLookup = {
        41: "NorthernAfrica",
         1: "Brazil",
         0: "Argentina",
         2: "Peru",
         3: "Venezuela",
         4: "Mexico",
        12: "EasternUS",
         5: "WesternUS",
         6: "Alaska",
         7: "NorthwestTerritory",
         8: "Alberta",
         9: "Ontario",
        11: "Quebec",
        10: "Greenland",
        15: "Iceland",
        16: "Scandinavia",
        17: "Ukraine",
        14: "NorthernEurope",
        13: "GreatBritain",
        40: "WesternEurope",
        39: "SouthernEurope",
        37: "Egypt",
        35: "Congo",
        34: "SouthAfrica",
        33: "Madagascar",
        36: "EasternAfrica",
        38: "MiddleEast",
        25: "Afghanistan",
        18: "Ural",
        19: "Siberia",
        20: "Yakutsk",
        21: "Kamchatka",
        22: "Irkutsk",
        24: "Mongolia",
        23: "Japan",
        27: "China",
        26: "India",
        28: "Siam",
        29: "Indonesia",
        30: "NewGuinea",
        31: "EasternAustralia",
        32: "WesternAustralia",
        42: "Wild1",
        43: "Wild2"
      }

      var colorLookup = {
        0: "red",
        1: "green",
        2: "blue",
        3: "yellow",
        4: "magenta",
        5: "cyan",
        255: "white" // No owner yet (very early in the game) (OR wild cards)
      }

      var typeLookup = {
        "Afghanistan" : "INFANTRY",
        "Alaska" : "ARTILLERY",
        "Alberta" : "ARTILLERY",
        "Argentina" : "INFANTRY",
        "Brazil" : "INFANTRY",
        "China" : "ARTILLERY",
        "Congo" : "ARTILLERY",
        "EasternAfrica" : "INFANTRY",
        "EasternAustralia" : "CAVALRY",
        "EasternUS" : "ARTILLERY",
        "Egypt" : "CAVALRY",
        "GreatBritain" : "INFANTRY",
        "Greenland" : "CAVALRY",
        "Iceland" : "CAVALRY",
        "India" : "CAVALRY",
        "Indonesia" : "INFANTRY",
        "Irkutsk" : "ARTILLERY",
        "Japan" : "CAVALRY",
        "Kamchatka" : "ARTILLERY",
        "Madagascar" : "CAVALRY",
        "Mexico" : "INFANTRY",
        "MiddleEast" : "ARTILLERY",
        "Mongolia" : "CAVALRY",
        "NewGuinea" : "CAVALRY",
        "NorthernAfrica" : "INFANTRY",
        "NorthernEurope" : "CAVALRY",
        "NorthwestTerritory" : "CAVALRY",
        "Ontario" : "ARTILLERY",
        "Peru" : "CAVALRY",
        "Quebec" : "ARTILLERY",
        "Scandinavia" : "INFANTRY",
        "Siam" : "CAVALRY",
        "Siberia" : "INFANTRY",
        "SouthAfrica" : "ARTILLERY",
        "SouthernEurope" : "INFANTRY",
        "Ukraine" : "INFANTRY",
        "Ural" : "INFANTRY",
        "Venezuela" : "CAVALRY",
        "WesternAustralia" : "ARTILLERY",
        "WesternEurope" : "INFANTRY",
        "WesternUS" : "ARTILLERY",
        "Wild1" : "WILD",
        "Wild2" : "WILD",
        "Yakutsk" : "ARTILLERY"
      }

/*
      var typeLookup = {
         0: "INFANTRY",
         1: "INFANTRY",
         2: "INFANTRY",
         3: "CAVALRY",
         4: "CAVALRY",
         5: "INFANTRY",
         6: "ARTILLERY",
         7: "ARTILLERY",
         8: "ARTILLERY",
         9: "CAVALRY",
        10: "ARTILLERY",
        11: "ARTILLERY",
        12: "ARTILLERY",
        13: "CAVALRY",
        14: "CAVALRY",
        15: "INFANTRY",
        16: "INFANTRY",
        17: "CAVALRY",
        18: "INFANTRY",
        19: "INFANTRY",
        20: "INFANTRY",
        21: "CAVALRY",
        22: "ARTILLERY",
        23: "ARTILLERY",
        24: "CAVALRY",
        25: "INFANTRY",
        26: "ARTILLERY",
        27: "INFANTRY",
        28: "INFANTRY",
        29: "INFANTRY",
        30: "ARTILLERY",
        31: "ARTILLERY",
        32: "ARTILLERY",
        33: "CAVALRY",
        34: "CAVALRY",
        35: "ARTILLERY",
        36: "CAVALRY",
        37: "CAVALRY",
        38: "INFANTRY",
        39: "CAVALRY",
        40: "CAVALRY",
        41: "ARTILLERY",
        42: "WILD",
        43: "WILD"
      }
*/
      var cards = [];
      var selectedCards = [];

      function updateCard(cardNum) {
        if (cardNum > 41) {
          return; // Wild card cannot change its background
        }
        var id = cardLookup[cardNum];
        backgroundColor = gamestate['territories'][cardNum];
        $('#' + id).css("background-color", colorLookup[backgroundColor]);
      }

      function addCard(cardNum) {
        // Don't add cards that you already have
        if ($.inArray(cardNum, cards) != -1) {
          return false;
        }

        // Add the new card
        cards.push(cardNum);
        var backgroundColor = 255;
        if (cardNum != 42 && cardNum != 43) {
          backgroundColor = gamestate['territories'][cardNum];
        }
        var id = cardLookup[cardNum];
        var imgsrc = 'src="img/' + id + '.png"';
        //console.log("adding a card (" + numCards + ") - " + imgsrc);
        $("#cardList").append('<li><img id="' + id + '" ' + imgsrc + ' ' + '></img></li>');
        numCards += 1;
        $("#" + id).css("background-color", colorLookup[backgroundColor]);
        $("#" + id).css("margin", "50px 0 0 0");
        $("#" + id).data("card_number", cardNum);
        $("#scroller").css("width", numCards * 188 + 10);
        $("#" + id).on("tap", function(event) {
          if (canClick == false) {
            return false;
          }
          if (event.handled !== true) {

            $(this).data("chosenCard", !$(this).data("chosenCard"));
            if ($(this).data("chosenCard")) {
              $(this).css("margin", "0 0 50px 0");
              selectedCards.push($(this).data("card_number"));
              // Check if 3 cards were selected
              if (selectedCards.length == 3) {
                // Attempt to submit cards
                // Check if the 3 cards are valid to submit
                if (cardsValid()) {
                  // Try to submit them (they may still be invalid if it isn't your turn)
                  $.post("submitCards.php", { "player" : player, "cards" : selectedCards }, function(data) {console.log(data);});
                }
                // Deselect the three cards. If they were used they will be removed on the next update (0.1s max)
                setTimeout(function() {
                  $("#" + cardLookup[selectedCards[0]]).css("margin", "50px 0 0 0").data("chosenCard", !$("#" + cardLookup[selectedCards[0]]).data("chosenCard"));
                  $("#" + cardLookup[selectedCards[1]]).css("margin", "50px 0 0 0").data("chosenCard", !$("#" + cardLookup[selectedCards[1]]).data("chosenCard"));
                  $("#" + cardLookup[selectedCards[2]]).css("margin", "50px 0 0 0").data("chosenCard", !$("#" + cardLookup[selectedCards[2]]).data("chosenCard"));
                  selectedCards = [];
                }, 250);
              }
            } else {
              $(this).css("margin", "50px 0 0 0");
              selectedCards.splice($.inArray($(this).data("card_number"), selectedCards), 1);
            }
            console.log(selectedCards.toString());
            event.handled = true;
          } else {
            return false;
          }
        });
        myScroll.refresh();
      }

      function removeCard(cardNum) {
        var id = cardLookup[cardNum];
        // Remove the card from the screen
        $("#" + id).remove();
        // Remove the card from the cards array so that it can be picked up again later
        cards.splice($.inArray(cardNum, cards), 1);
        // Remove the card from the selected array if it is in there
        selectedCards.splice($.inArray(cardNum, selectedCards), 1);
        myScroll.refresh();
      }

      function cardsValid() {
        if (selectedCards.length != 3) {
          return false;
        }
        var types = [ typeLookup[cardLookup[selectedCards[0]]], typeLookup[cardLookup[selectedCards[1]]], typeLookup[cardLookup[selectedCards[2]]] ];
        types.sort();
        var numWild = 0;
        if ($.inArray("WILD", types) != -1) {
          // Any combination involving at least one WILD is valid
          return true;
        }
        // Otherwise check if all 3 are the same
        if (types[0] == types[1] && types[1] == types[2]) {
          return true;
        }
        // Last chance, check if all 3 cards are different
        if (types[0] != types[1] && types[0] != types[2] && types[1] != types[2]) {
          return true;
        }
        // If none of these cases match, then return false
        return false;
      }

      var gamestate;
      function update() {
        if (player === undefined) {
          console.log("pick a color!");
          return false;
        }
        $.getJSON("gamestate.json", function(data) {
          console.log("Received updated gamestate information.");
          gamestate = data;
          for (i = 0; i < gamestate['cards'].length; i++) { //> ending poor syntax highlighting
            //console.log("  Checking if player " + player.toString() + " owns card " + i.toString());
            if (gamestate['cards'][i] == player) {
              // Check if this card is already displayed
              if ($.inArray(i, cards) != -1) {
                updateCard(i);
              } else {
                addCard(i);
              }
            } else if ($.inArray(i, cards) != -1) { // If not your card, but it is displayed
              // Remove any cards that you no longer have
              removeCard(i);
            }
          }
        });
      }

      function toggleMode() {
        if (mode == "picker") {
          $("#switch").css('visibility', 'visible');
        }
        if (mode == "cards") {
          mode = "statistics";
          $("#wrapper").css('visibility', 'hidden');
          $("#statistics").css('visibility', 'visible');
          $("#switch").html("Show Cards");
        } else {
          mode = "cards";
          $("#statistics").css('visibility', 'hidden');
          $("#wrapper").css('visibility', 'visible');
          $("#switch").html("Show Statistics");
        }
      }

var stats;

function updateStatistics() {
  console.log("Updating statistics");
  // Request stats
  $.getJSON("statistics.json", function(data) {
    stats = data;
    var totalDice = data[player]["dice"];
    $("#totalDice").html("Dice Rolled: " + data[player]["dice"]);
    $("#ones").html("Ones: " + (100 * data[player]["ones"] / totalDice).toFixed(2) + "%");
    $("#twos").html("Twos: " + (100 * data[player]["twos"] / totalDice).toFixed(2) + "%");
    $("#threes").html("Threes: " + (100 * data[player]["threes"] / totalDice).toFixed(2) + "%");
    $("#fours").html("Fours: " + (100 * data[player]["fours"] / totalDice).toFixed(2) + "%");
    $("#fives").html("Fives: " + (100 * data[player]["fives"] / totalDice).toFixed(2) + "%");
    $("#sixes").html("Sixes: " + (100 * data[player]["sixes"] / totalDice).toFixed(2) + "%");
    $("#territory").html("Territory: " + cardLookup[data[player]["territory"]] + " (" + data[player]["turns"]  + " turns)");
    $("#troops").html("Troops deployed: " + data[player]["troops"]);
    $("#kills").html("Kills: " + data[player]["kills"]);
    $("#deaths").html("Deaths: " + data[player]["deaths"]);
    $("gameterritory").html("Longest Owned Territory: " + data["territory"] + " (" + data["turns"] + " turns)");
  });
}

document.addEventListener('touchmove', function(e) { e.preventDefault(); }, false);

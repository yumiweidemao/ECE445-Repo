const usageList = document.getElementById('usageList');
const rakingList = document.getElementById('rakingList');
const odorLevel = document.getElementById('odorLevel');
const body = document.body;

function addUsageMessage(message) {
  const li = document.createElement('li');
  li.classList.add('list-group-item');
  li.textContent = message;
  usageList.prepend(li);

  // Keeping only last 10 messages
  while (usageList.children.length > 10) {
    usageList.removeChild(usageList.lastChild);
  }
}

function addRakingMessage(message) {
  const li = document.createElement('li');
  li.classList.add('list-group-item');
  li.textContent = message;
  rakingList.prepend(li);

  // Keeping only last 5 messages
  while (rakingList.children.length > 5) {
    rakingList.removeChild(rakingList.lastChild);
  }
}

function updateOdorLevel(level) {
  odorLevel.textContent = level;
}

function rake() {
  body.classList.add('rake-effect');

  setTimeout(() => {
    body.classList.remove('rake-effect');
    body.classList.add('rake-effect-back');
  }, 1000);

  setTimeout(() => {
    body.classList.remove('rake-effect-back');
  }, 2000);

  addRakingMessage('Raking initiated...'); // Example
}

// Example usage
addUsageMessage('Message 1');
addUsageMessage('Message 2');
addRakingMessage('Rake 1');
updateOdorLevel('Low');

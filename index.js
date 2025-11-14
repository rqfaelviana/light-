document.addEventListener("DOMContentLoaded", () => {
  const umbrella = document.querySelector(".hero-umbrella");
  const audio = document.getElementById("frevo-audio");

  let isSpining = false;

  if (umbrella && audio) {
    umbrella.addEventListener("click", () => {
      if (isSpining) {
        return;
      }

      isSpining = true;
      audio.currentTime = 0;

      audio.play();

      umbrella.classList.add("spin");

      setTimeout(() => {
        umbrella.classList.remove("spin");
        isSpining = false;
      }, 600);
    });
  }
});

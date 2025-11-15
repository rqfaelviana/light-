document.addEventListener("DOMContentLoaded", () => {
  // ANIMAÇÕES GSAP — Entrada elegante da seção hero

  gsap.from(".hero-titulo", {
    duration: 1.2,
    y: -40,
    opacity: 0,
    ease: "power3.out",
  });

  gsap.from(".hero-umbrella", {
    duration: 1.3,
    delay: 0.2,
    opacity: 0,
    rotate: -40,
    y: -20,
    ease: "back.out(1.7)",
  });

  gsap.from(".hero-botoes", {
    duration: 1.2,
    delay: 0.35,
    y: 40,
    opacity: 0,
    ease: "power3.out",
  });

  gsap.from(".lead", {
    duration: 1.2,
    delay: 0.6,
    opacity: 0,
    y: 30,
    ease: "power3.out",
  });

  gsap.from(".actions", {
    duration: 1.2,
    delay: 0.8,
    opacity: 0,
    y: 30,
    ease: "power3.out",
  });
  const header = document.querySelector("header");

  setTimeout(() => {
    header.classList.add("show");
  }, 150);
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

      confetti({
        particleCount: 150,
        spread: 90,
        origin: { y: 0.6 },
      });

      setTimeout(() => {
        umbrella.classList.remove("spin");
        isSpining = false;
      }, 600);
    });
  }
});

export class RandomFactImageRequest {
  url: String;
  fact: String;
  lang: String;

  constructor(url: String, fact: String, lang: String) {
    this.url = url;
    this.fact = fact;
    this.lang = lang;
  }
}

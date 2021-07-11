import { Component } from '@angular/core';

import { FactService } from 'src/app/services/fact.service';

import { RandomFactImageRequest } from 'src/app/models/RandomFactImageRequest';
import { RandomFactImageResponse } from 'src/app/models/RandomFactImageResponse';
import { RandomFactResponse } from 'src/app/models/RandomFactResponse';
import { RandomImageResponse } from 'src/app/models/RandomImageResponse';
import { RandomFactRequest } from 'src/app/models/RandomFactRequest';

@Component({
  selector: 'app-main-page',
  templateUrl: './main-page.component.html',
  styleUrls: ['./main-page.component.scss'],
})
export class MainPageComponent {
  public randomImageUrl: String = null;
  public randomFact: String = null;
  public randomFactImageUrl: String = null;
  public errorMessage: String = null;

  public selectedLanguage: String = 'en';
  public languages: Object[] = [
    {
      display: 'English',
      code: 'en',
    },
    {
      display: 'French',
      code: 'fr',
    },
    {
      display: 'Spanish',
      code: 'es',
    },
    {
      display: 'Romanian',
      code: 'ro',
    },
  ];

  constructor(private factsService: FactService) {}

  public getRandomImage() {
    this.factsService.getRandomImage().subscribe(
      (image: RandomImageResponse) => {
        this.randomImageUrl = image.url;
        this.clearErrorMessage();
      },
      (error) => {
        this.setErrorMessageFromErrorObject(error);
      }
    );
  }

  public getRandomFact() {
    let randomFactRequest: RandomFactRequest = new RandomFactRequest(
      this.selectedLanguage
    );
    this.factsService.getRandomFact(randomFactRequest).subscribe(
      (fact: RandomFactResponse) => {
        this.randomFact = fact.fact;
        this.clearErrorMessage();
      },
      (error) => {
        this.setErrorMessageFromErrorObject(error);
      }
    );
  }

  public getFactImage() {
    let randomFactImageRequest: RandomFactImageRequest = new RandomFactImageRequest(
      this.randomImageUrl,
      this.randomFact,
      this.selectedLanguage
    );
    this.factsService.buildFactImage(randomFactImageRequest).subscribe(
      (imageData: RandomFactImageResponse) => {
        this.randomFactImageUrl = imageData.image;
        this.randomImageUrl = imageData.randomImage;
        this.randomFact = imageData.fact;
        this.clearErrorMessage();
      },
      (error) => {
        this.setErrorMessageFromErrorObject(error);
      }
    );
  }

  private setErrorMessageFromErrorObject(error) {
    if (!error || !('error' in error) || !('message' in error['error'])) {
      this.setErrorMessage('Server error!');
    } else {
      this.setErrorMessage(error['error']['message']);
    }
  }

  private setErrorMessage(message: String) {
    this.errorMessage = message;
    setTimeout(
      (component) => {
        component.clearErrorMessage();
      },
      3000,
      this
    );
  }

  private clearErrorMessage() {
    this.errorMessage = null;
  }
}

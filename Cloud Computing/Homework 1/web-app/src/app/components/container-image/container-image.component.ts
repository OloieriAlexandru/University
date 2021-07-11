import { Component, Input } from '@angular/core';

@Component({
  selector: 'app-container-image',
  templateUrl: './container-image.component.html',
  styleUrls: ['./container-image.component.scss'],
})
export class ContainerImageComponent {
  @Input() imageToDisplay: String = null;

  constructor() {}
}

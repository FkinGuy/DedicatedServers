import { CognitoIdentityProviderClient, AdminGetUserCommand } from "@aws-sdk/client-cognito-identity-provider";
import { DynamoDBClient, GetItemCommand, PutItemCommand } from "@aws-sdk/client-dynamodb";
import { marshall } from "@aws-sdk/util-dynamodb";

export const handler = async (event) => {
  const cognitoIdentityProviderClient = new CognitoIdentityProviderClient( { region: process.env.REGION } );
  const dynamoDBClient = new DynamoDBClient( { region: process.env.REGION } );
  try {
    const adminGetUserInput = {
      Username: event.username,
      UserPoolId: process.env.USER_POOL_ID
    };
    const adminGetUserCommand = new AdminGetUserCommand(adminGetUserInput);
    const adminGetUserResponse = await cognitoIdentityProviderClient.send(adminGetUserCommand);

    const sub = adminGetUserResponse.UserAttributes.find(attribute => attribute.Name === "sub").Value;
    const email = adminGetUserResponse.UserAttributes.find(attribute => attribute.Name === "email").Value;
    
    const getItemInput = {
      TableName: "Players",
      Key: marshall( { databaseid: sub } ),
    };

    const getItemCommand = new GetItemCommand(getItemInput);
    const existingStats = await dynamoDBClient.send(getItemCommand);

    let matchStats = event.matchStats;

    for (const key in matchStats) {
      const value = matchStats[key];
      if (existingStats[key] !== undefined) {
        matchStats[key] += existingStats[key];
      }
    }

    const putItemInput = {
      TableName: "Players",
      Item: marshall( 
        {
          databaseid: sub,
          username: event.username,
          "email": email,
          ...matchStats,
        }
      )
    };
    const putItemCommand = new PutItemCommand(putItemInput);
    await dynamoDBClient.send(putItemCommand);
    
    return {
      statusCode: 200,
      body: `Updated match stats for ${event.username}`,
    }

  } catch(error) {
    return error;
  }
};
